class RoomServices {
  constructor(RoomModel) {
    this.RoomModel = RoomModel;
  }

  createRoom = async (roomData) => {
    if(!roomData.name || !roomData.microcontrollerId){
      throw new Error("Nome e microcontrollerId são obrigatórios");
    }

    const room = new this.RoomModel(roomData);
    return await room.save();
  }

  getAllRooms = async () => {
    try {
      return await this.RoomModel.find();
    } catch (error) {
      throw new Error("Erro ao buscar salas: " + error.message);
    }
  }

  getRoomById = async (roomId) => {
    if (!roomId) {
      throw new Error("roomId é obrigatório");
    }
    try {
      const room = await this.RoomModel.findById(roomId);
      if (!room) {
        throw new Error("Sala não encontrada");
      }
      return room;
    } catch (error) {
      throw new Error("Erro ao buscar sala por ID: " + error.message);
    }
  }

  getRoomByMicrocontrollerId = async (microcontrollerId) => {
    if (!microcontrollerId) {
      throw new Error("microcontrollerId é obrigatório");
    }
    try {
      const room = await this.RoomModel.findOne({ microcontrollerId });
      if (!room) {
        throw new Error("Sala não encontrada para o microcontrollerId fornecido");
      }
      return room;
    } catch (error) {
      throw new Error("Erro ao buscar sala por microcontrollerId: " + error.message);
    }
  }

  updateRoom = async (roomId, updateData) => {
    if (!roomId) {
      throw new Error("roomId é obrigatório");
    }
    if (!updateData || Object.keys(updateData).length === 0) {
      throw new Error("Dados para atualização são obrigatórios");
    }
    try {
      const updatedRoom = await this.RoomModel.findByIdAndUpdate(roomId, updateData, { new: true });
      if (!updatedRoom) {
        throw new Error("Sala não encontrada para atualização");
      }
      return updatedRoom;
    } catch (error) {
      throw new Error("Erro ao atualizar sala: " + error.message);
    }
  }

  deleteRoom = async (roomId) => {
    if (!roomId) {
      throw new Error("roomId é obrigatório");
    }
    try {
      const deletedRoom = await this.RoomModel.findByIdAndDelete(roomId);
      if (!deletedRoom) {
        throw new Error("Sala não encontrada para exclusão");
      }
      return deletedRoom;
    } catch (error) {
      throw new Error("Erro ao excluir sala: " + error.message);
    }
  }

}

export default RoomServices;