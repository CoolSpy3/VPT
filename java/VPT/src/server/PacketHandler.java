package server;

import common.networking.packet.Packet;
import common.networking.packet.PacketId;
import common.networking.packet.packets.CreateUserPacket;
import common.networking.packet.packets.DeleteUserPacket;
import common.networking.packet.packets.LoginPacket;
import common.networking.packet.packets.result.DefaultResults;
import common.networking.packet.packets.result.ErrorResultPacket;
import common.networking.packet.packets.result.ResultPacket;
import common.networking.ssl.SSLConnection;
import java.io.IOException;
import java.time.Duration;
import server.user.LoginService;
import server.user.User;
import server.user.UserStore;

/**
 * Generates {@link ResultPacket}s in response to client requests
 * @see ConnectionHandler
 */
public final class PacketHandler {
    
    /**
     * Processes a client request
     * @param p the Packet request from the client
     * @param onUserDeletion a method to be run if the currently logged in user is logged out
     * @param connection the SSLConnection associated with the client
     * @return a {@link ResultPacket} responding to the client's request
     */
    public static ResultPacket process(Packet p, Runnable onUserDeletion, SSLConnection connection) {
        try {
            if(p.id == PacketId.LOGIN.id) {
                RequestService.request(connection, "Login", ServerConstants.USER_SPEC_REQUESTS_TE);
                User currentUser = LoginService.getCurrentUser();
                if(currentUser != null) {
                    LoginService.logout();
                    UserStore.unsubscribeFromDeletionEvents(currentUser.userId, onUserDeletion);
                }
                LoginPacket loginPacket = (LoginPacket)p;
                boolean result = LoginService.login(loginPacket.userId, loginPacket.password);
                if(result) {
                    UserStore.subscribeToDeletionEvents(loginPacket.userId, onUserDeletion);
                }
                return DefaultResults.login(result);
            } else if(p.id == PacketId.CREATE_USER.id) {
                RequestService.request(connection, "Create User", ServerConstants.USER_SPEC_REQUESTS_TE);
                try {
                    CreateUserPacket packet = (CreateUserPacket)p;
                    UserStore.createUser(new User(packet.userId, packet.password, packet.isAdmin));
                    return DefaultResults.createUser(true);
                } catch(IllegalArgumentException e) {
                    return DefaultResults.createUser(false, e.getMessage());
                }
            } else if(p.id == PacketId.DELETE_USER.id) {
                RequestService.request(connection, "Delete User", ServerConstants.USER_ONET_REQUESTS_TE);
                try {
                    DeleteUserPacket packet = (DeleteUserPacket)p;
                    UserStore.deleteUser(packet.data);
                    return DefaultResults.deleteUser(true);
                } catch(IllegalArgumentException e) {
                    return DefaultResults.deleteUser(false, e.getMessage());
                }
            } else if(p.id == PacketId.SHUTDOWN.id) {
                System.exit(0);
            }
            return ErrorResultPacket.INVALID_REQUEST;
        } catch(RequestService.TooManyRequestsException e) {
            return ErrorResultPacket.TOO_MANY_REQUESTS(formatTimeout(e.timeout));
        } catch(SecurityException e) {
            return ErrorResultPacket.ILLEGAL_ACCESS(e.getMessage());
        } catch(ClassCastException e) {
            return ErrorResultPacket.INVALID_REQUEST;
        } catch(IOException e) {
            return ErrorResultPacket.SERVER_ERROR;
        }
    }
    
    /**
     * Formats a timeout caused by too many client requests
     * @param timeout the timeout in nanoseconds
     * @return a String representing the timeout in more conventional units
     */
    public static String formatTimeout(long timeout) {
        Duration duration = Duration.ofNanos(timeout);
        long days = duration.toDaysPart();
        int hours = duration.toHoursPart();
        int minutes = duration.toMinutesPart();
        int seconds = duration.toSecondsPart();
        String out = "";
        if(days != 0) {
            out += days;
            out += " Days ";
        }
        if(hours != 0) {
            out += hours;
            out += " Hours ";
        }
        if(minutes != 0) {
            out += minutes;
            out += " Minutes ";
        }
        if(seconds != 0) {
            out += seconds;
            out += " Seconds ";
        }
        return out.trim();
    }
    
    private PacketHandler() {}
    
}