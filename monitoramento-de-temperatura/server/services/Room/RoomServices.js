import ApiError from "../../utils/errors.js";

class RoomServices {
  constructor(RoomModel) {
    this.RoomModel = RoomModel;
  }

  createRoom = async (roomData) => {
    if (!roomData.name || !roomData.microcontrollerId) {
      throw ApiError.badRequest("Nome e microcontrollerId são obrigatórios");
    }

    const room = new this.RoomModel(roomData);
    return await room.save();
  }

  getAllRooms = async () => {
    const rooms = await this.RoomModel.find();
    return rooms;
  }

  getRoomById = async (roomId) => {
    if (!roomId) {
      throw ApiError.badRequest("roomId é obrigatório");
    }

    const room = await this.RoomModel.findById(roomId);
    if (!room) {
      throwApiError.badRequest("Sala não encontrada");
    }
    return room;
  }

  getRoomByMicrocontrollerId = async (microcontrollerId) => {
    if (!microcontrollerId) {
      throw ApiError.badRequest("microcontrollerId é obrigatório");
    }

    const room = await this.RoomModel.findOne({ microcontrollerId });
    if (!room) {
      throw ApiError.badRequest("Sala não encontrada para o microcontrollerId fornecido");
    }
    return room;

  }

  updateRoom = async (roomId, updateData) => {
    if (!roomId) {
      throw ApiError.badRequest("roomId é obrigatório");
    }
    if (!updateData || Object.keys(updateData).length === 0) {
      throw ApiError.badRequest("Dados para atualização são obrigatórios");
    }

    const room = await this.RoomModel.findById(roomId);
    if (!room) {
      throw ApiError.badRequest("Sala não encontrada para atualização");
    }

    const updatedRoom = await this.RoomModel.findByIdAndUpdate(roomId, updateData, { new: true });
    return updatedRoom;
  }

  deleteRoom = async (roomId) => {
    if (!roomId) {
      throw ApiError.badRequest("roomId é obrigatório");
    }
    const room = await this.RoomModel.findById(roomId);

    if (!room) {
      throw ApiError.badRequest("Sala não encontrada para exclusão");
    }

    const deletedRoom = await this.RoomModel.findByIdAndDelete(roomId);

    return deletedRoom;
  }

}

export default RoomServices;