package common.networking.packet.packets;

import common.networking.packet.Packet;
import common.networking.packet.PacketId;

public class SingleDataPacket<T> extends Packet {
    
    private static final long serialVersionUID = 6527268264781500815L;
    
    public final T data;

    public SingleDataPacket(PacketId id, T data) {
        this(id.id, data);
    }

    public SingleDataPacket(int id, T data) {
        super(id);
        this.data = data;
    }
    
}