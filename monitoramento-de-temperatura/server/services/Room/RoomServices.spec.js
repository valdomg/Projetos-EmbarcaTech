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

});
