package server.user;

import common.Utils;
import common.user.NetPublicUser;
import java.io.File;
import java.io.IOException;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import server.ServerConstants;
import server.serialization.DefaultSerialization;

public final class UserStore {

    private static final ConcurrentHashMap<String, WeakReference<User>> users = new ConcurrentHashMap<>();
    private static final ReentrantReadWriteLock userLock = new ReentrantReadWriteLock(ServerConstants.USE_FAIR_LOCKS);
    private static final ConcurrentHashMap<String, ArrayList<UserAttribute>> publicAttributes = new ConcurrentHashMap<>();
    private static final ReentrantReadWriteLock attributeLock = new ReentrantReadWriteLock(ServerConstants.USE_FAIR_LOCKS);
    private static final ConcurrentHashMap<String, ArrayList<Runnable>> deletionSubscribers = new ConcurrentHashMap();
    private static final ReentrantReadWriteLock deletionSubscribersLock = new ReentrantReadWriteLock(ServerConstants.USE_FAIR_LOCKS);
    
    public static NetPublicUser getPublicUser(String userId) {
        return getUserInternal(userId).toNetPublicUser();
    }
    
    public static User getUser(String userId) throws SecurityException {
        User user = getUserInternal(userId);
        LoginService.checkAccess(user);
        return user;
    }
    
    public static User login(String userId, byte[] password) {
        User user = getUserInternal(userId);
        return user != null && user.checkPassword(password) ? user : null;
    }
    
    private static User getUserInternal(String userId) {
        userLock.readLock().lock();
        try {
            if(!users.containsKey(userId) || users.get(userId).get() == null) {
                if(!checkUserIdExistance(userId)) {
                    return null;
                }
                userLock.readLock().unlock();
                userLock.writeLock().lock();
                userLock.readLock().lock();
                try {
                    if(!checkUserIdExistance(userId)) {
                        return null;
                    }
                    User user;
                    try {
                        user = (User)DefaultSerialization.deserialize("Users/" + Utils.hash(userId) + ".usr");
                    } catch(ClassCastException | ClassNotFoundException | IOException e) {
                        return null;
                    }
                    if(user == null) {
                        return null;
                    }
                    users.put(userId, new WeakReference<>(user));
                    return user;
                } finally {
                    userLock.writeLock().unlock();
                }
            } else {
                return users.get(userId).get();
            }
        } finally {
            userLock.readLock().unlock();
        }
    }
    
    public static void subscribeToDeletionEvents(String userId, Runnable onUserDeletion) throws IllegalArgumentException {
        deletionSubscribersLock.readLock().lock();
        try {
            if(getUserInternal(userId) == null) {
                throw new IllegalArgumentException("User: " + userId + " does not exist");
            }
            synchronized(deletionSubscribers) {
                if(!deletionSubscribers.containsKey(userId)) {
                    deletionSubscribers.put(userId, new ArrayList<>());
                }
            }
            synchronized(deletionSubscribers.get(userId)) {
                deletionSubscribers.get(userId).add(onUserDeletion);
            }
        } finally {
            deletionSubscribersLock.readLock().unlock();
        }
    }
    
    public static void unsubscribeFromDeletionEvents(String userId, Runnable callback) {
        deletionSubscribersLock.readLock().lock();
        try {
            synchronized(deletionSubscribers) {
                if(!deletionSubscribers.containsKey(userId)) {
                    return;
                }
            }
            synchronized(deletionSubscribers.get(userId)) {
                deletionSubscribers.get(userId).remove(callback);
            }
        } finally {
            deletionSubscribersLock.readLock().unlock();
        }
    }
    
    public static boolean checkUserIdExistance(String userId) {
        userLock.readLock().lock();
        try {
            return new File(ServerConstants.SERVER_DIR + File.separator + "Users" + File.separator + Utils.hash(userId) + ".usr").exists();
        } finally {
            userLock.readLock().unlock();
        }
    }
    
    public static void createUser(User user) throws IllegalArgumentException, IOException, SecurityException {
        LoginService.checkAccess();
        userLock.writeLock().lock();
        try {
            if(checkUserIdExistance(user.userId)) {
                throw new IllegalArgumentException("User Already Exists");
            }
            DefaultSerialization.serialize(user, "Users/" + Utils.hash(user.userId) + ".usr");
            users.put(user.userId, new WeakReference<>(user));
        } finally {
            userLock.writeLock().unlock();
        }
    }
    
    private UserStore() {}

}