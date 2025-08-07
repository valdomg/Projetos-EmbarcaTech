class RoomServices {
  constructor(RoomModel) {
    this.RoomModel = RoomModel;
  }

  createRoom = async (roomData) => {
    const room = new this.RoomModel(roomData);
    return await room.save();
  }

  getAllRooms = async () => {
    return await this.RoomModel.find();
  }

  getRoomById = async (roomId) => {
    return await this.RoomModel.findById(roomId);
  }

  getRoomByMicrocontrollerId = async (microcontrollerId) => {
    return await this.RoomModel.findOne({ microcontrollerId });
  }

  updateRoom = async (roomId, updateData) => {
    return await this.RoomModel.findByIdAndUpdate(roomId, updateData, { new: true });
  }

  deleteRoom = async (roomId) => {
    return await this.RoomModel.findByIdAndDelete(roomId);
  }
}

export default RoomServices;