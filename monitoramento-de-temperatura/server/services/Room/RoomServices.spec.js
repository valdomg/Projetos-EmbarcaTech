import RoomServices from "./RoomServices.js";
import { jest } from '@jest/globals';

const mockModel = {
  find: jest.fn(),
  findById: jest.fn(),
  findByIdAndDelete: jest.fn(),
  create: jest.fn(),
};

describe('RoomServices', () => {
  let service;
  let saveMock;

  beforeEach(() => {
    saveMock = jest.fn().mockResolvedValue('mocked saved reading');
    const model = function (data) {
      return {
        ...data,
        save: saveMock
      };
    };

    model.find = mockModel.find;
    model.findById = mockModel.findById;
    model.findByIdAndDelete = mockModel.findByIdAndDelete;

    service = new RoomServices(model);

    jest.clearAllMocks();
  });

  describe('createRoom', () => {
    it('should save room correctly', async () => {
      const data = {
        name: 'Sala',
        microcontrollerId: 'abc123',
        minTemperature: 18,
        maxTemperature: 26
      };

      const result = await service.createRoom(data);

      expect(saveMock).toHaveBeenCalled();
      expect(result).toBe('mocked saved reading');
    });

    it('should throw an error if name is missing', async () => {
      const data = {
        microcontrollerId: 'abc123',
        minTemperature: 18,
        maxTemperature: 26
      };

      await expect(service.createRoom(data)).rejects.toThrow("Nome e microcontrollerId são obrigatórios");
    });

    it('should throw an error if microcontrollerId is missing', async () => {
      const data = {
        name: 'Sala',
        minTemperature: 18,
        maxTemperature: 26
      };

      await expect(service.createRoom(data)).rejects.toThrow("Nome e microcontrollerId são obrigatórios");
    });
  });

  describe('getAllRooms', () => {
    it('should return all rooms', async () => {
      mockModel.find.mockResolvedValue(['room1', 'room2']);
      const result = await service.getAllRooms();
      expect(mockModel.find).toHaveBeenCalled();
      expect(result).toEqual(['room1', 'room2']);
    });

    it('should throw error if find fails', async () => {
      mockModel.find.mockRejectedValue(new Error('DB error'));
      await expect(service.getAllRooms()).rejects.toThrow('Erro ao buscar salas: DB error');
    });
  });

  describe('getRoomById', () => {
    it('should return room by id', async () => {
      mockModel.findById.mockResolvedValue('room1');
      const result = await service.getRoomById('id123');
      expect(mockModel.findById).toHaveBeenCalledWith('id123');
      expect(result).toBe('room1');
    });

    it('should throw error if roomId is missing', async () => {
      await expect(service.getRoomById()).rejects.toThrow('roomId é obrigatório');
    });

    it('should throw error if room not found', async () => {
      mockModel.findById.mockResolvedValue(null);
      await expect(service.getRoomById('id123')).rejects.toThrow('Sala não encontrada');
    });

    it('should throw error if findById fails', async () => {
      mockModel.findById.mockRejectedValue(new Error('DB error'));
      await expect(service.getRoomById('id123')).rejects.toThrow('Erro ao buscar sala por ID: DB error');
    });
  });

  describe('getRoomByMicrocontrollerId', () => {
    it('should return room by microcontrollerId', async () => {
      mockModel.findOne = jest.fn().mockResolvedValue('room1');
      service.RoomModel.findOne = mockModel.findOne;
      const result = await service.getRoomByMicrocontrollerId('mc123');
      expect(mockModel.findOne).toHaveBeenCalledWith({ microcontrollerId: 'mc123' });
      expect(result).toBe('room1');
    });

    it('should throw error if microcontrollerId is missing', async () => {
      await expect(service.getRoomByMicrocontrollerId()).rejects.toThrow('microcontrollerId é obrigatório');
    });

    it('should throw error if room not found', async () => {
      mockModel.findOne = jest.fn().mockResolvedValue(null);
      service.RoomModel.findOne = mockModel.findOne;
      await expect(service.getRoomByMicrocontrollerId('mc123')).rejects.toThrow('Sala não encontrada para o microcontrollerId fornecido');
    });

    it('should throw error if findOne fails', async () => {
      mockModel.findOne = jest.fn().mockRejectedValue(new Error('DB error'));
      service.RoomModel.findOne = mockModel.findOne;
      await expect(service.getRoomByMicrocontrollerId('mc123')).rejects.toThrow('Erro ao buscar sala por microcontrollerId: DB error');
    });
  });

  describe('updateRoom', () => {
    it('should update room correctly', async () => {
      mockModel.findByIdAndUpdate = jest.fn().mockResolvedValue('updatedRoom');
      service.RoomModel.findByIdAndUpdate = mockModel.findByIdAndUpdate;
      const result = await service.updateRoom('id123', { name: 'Nova Sala' });
      expect(mockModel.findByIdAndUpdate).toHaveBeenCalledWith('id123', { name: 'Nova Sala' }, { new: true });
      expect(result).toBe('updatedRoom');
    });

    it('should throw error if roomId is missing', async () => {
      await expect(service.updateRoom(undefined, { name: 'Nova Sala' })).rejects.toThrow('roomId é obrigatório');
    });

    it('should throw error if updateData is missing', async () => {
      await expect(service.updateRoom('id123')).rejects.toThrow('Dados para atualização são obrigatórios');
      await expect(service.updateRoom('id123', {})).rejects.toThrow('Dados para atualização são obrigatórios');
    });

    it('should throw error if room not found for update', async () => {
      mockModel.findByIdAndUpdate = jest.fn().mockResolvedValue(null);
      service.RoomModel.findByIdAndUpdate = mockModel.findByIdAndUpdate;
      await expect(service.updateRoom('id123', { name: 'Nova Sala' })).rejects.toThrow('Sala não encontrada para atualização');
    });

    it('should throw error if findByIdAndUpdate fails', async () => {
      mockModel.findByIdAndUpdate = jest.fn().mockRejectedValue(new Error('DB error'));
      service.RoomModel.findByIdAndUpdate = mockModel.findByIdAndUpdate;
      await expect(service.updateRoom('id123', { name: 'Nova Sala' })).rejects.toThrow('Erro ao atualizar sala: DB error');
    });
  });

  describe('deleteRoom', () => {
    it('should delete room correctly', async () => {
      mockModel.findByIdAndDelete.mockResolvedValue('deletedRoom');
      const result = await service.deleteRoom('id123');
      expect(mockModel.findByIdAndDelete).toHaveBeenCalledWith('id123');
      expect(result).toBe('deletedRoom');
    });

    it('should throw error if roomId is missing', async () => {
      await expect(service.deleteRoom()).rejects.toThrow('roomId é obrigatório');
    });

    it('should throw error if room not found for deletion', async () => {
      mockModel.findByIdAndDelete.mockResolvedValue(null);
      await expect(service.deleteRoom('id123')).rejects.toThrow('Sala não encontrada para exclusão');
    });

    it('should throw error if findByIdAndDelete fails', async () => {
      mockModel.findByIdAndDelete.mockRejectedValue(new Error('DB error'));
      await expect(service.deleteRoom('id123')).rejects.toThrow('Erro ao excluir sala: DB error');
    });
  });

});
