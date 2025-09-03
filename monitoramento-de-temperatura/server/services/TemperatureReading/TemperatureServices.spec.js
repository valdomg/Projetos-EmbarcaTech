import TemperatureService from "./TemperatureServices.js";
import { jest } from '@jest/globals';

const mockModel = {
  find: jest.fn(),
  findById: jest.fn(),
  findByIdAndDelete: jest.fn(),
  create: jest.fn(),
};

const mockRoomService = {
  getRoomByMicrocontrollerId: jest.fn(),
};

describe('TemperatureService', () => {
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

    service = new TemperatureService(model);
    service.roomService = mockRoomService;

    jest.clearAllMocks();
  });

  describe('createTemperatureReading', () => {
    it('should save temperature reading correctly', async () => {
      const data = {
        temperature: 22,
        room: 'Sala',
        humidity: 50,
        microcontrollerId: 'abc123'
      };

      mockRoomService.getRoomByMicrocontrollerId.mockResolvedValue({ _id: 'mockRoomId' });

      const result = await service.createTemperatureReading(data);

      expect(saveMock).toHaveBeenCalled();
      expect(result).toBe('mocked saved reading');
    });

    test.todo("Caso de falha ao criar leitura com campo faltando");

    test.todo("Caso de falha ao criar leitura com dados inválidos");

    test.todo("Caso de falha ao criar leitura com microcontrolador não cadastrado");
  });

  describe('getTemperatureReadings', () => {
    it('should fetch all readings', async () => {
      mockModel.find.mockReturnValueOnce({
        populate: jest.fn().mockResolvedValue(['reading1', 'reading2'])
      });

      const result = await service.getTemperatureReadings();

      expect(mockModel.find).toHaveBeenCalledWith();
      expect(result).toEqual(['reading1', 'reading2']);
    });

    it('should return an empty array if no readings found', async () => {
      mockModel.find.mockReturnValueOnce({
        populate: jest.fn().mockResolvedValue([])
      });

      const result = await service.getTemperatureReadings();

      expect(mockModel.find).toHaveBeenCalledWith();
      expect(result).toEqual([]);
    });
  });

  describe('getTemperatureReadingsByInterval', () => {
    it('should return readings between two dates', async () => {
      const startDate = '2025-07-29T14:00:00Z';
      const endDate = '2025-07-30T14:00:00Z';
      const mockResult = ['reading in interval'];

      mockModel.find.mockResolvedValueOnce(mockResult);

      const result = await service.getTemperatureReadingsByInterval(startDate, endDate);

      expect(mockModel.find).toHaveBeenCalledWith({
        timestamp: {
          $gte: new Date(startDate),
          $lte: new Date(endDate)
        }
      });
      expect(result).toBe(mockResult);
    });

  it("should return an error if the interval is invalid", async () => {
    const startDate = '2025-07-30T14:00:00Z';
    const endDate = '2025-07-29T14:00:00Z';

    await expect(service.getTemperatureReadingsByInterval(startDate, endDate)).rejects.toThrow("Intervalo de datas inválido");
  });
  });

  describe('getRoomTemperatureReadings', () => {
    it('should return readings for a specific room', async () => {
      const mockResult = ['room reading'];
      mockModel.find.mockResolvedValueOnce(mockResult);

      const result = await service.getRoomTemperatureReadings('Sala');

      expect(mockModel.find).toHaveBeenCalledWith({ room: 'Sala' });
      expect(result).toBe(mockResult);
    });

    it('should return an error if the room does not exist', async () => {
      mockRoomService.getRoomByMicrocontrollerId.mockResolvedValue(null);

      await expect(service.getRoomTemperatureReadings('Sala')).rejects.toThrow("Sala não encontrada");
    });
  });

  describe('getRoomTemperatureReadingsByInterval', () => {
    it('should return readings for a room between two dates', async () => {
      const startDate = '2025-07-29T00:00:00Z';
      const endDate = '2025-07-30T00:00:00Z';
      const mockResult = ['room + interval reading'];

      mockModel.find.mockResolvedValueOnce(mockResult);

      const result = await service.getRoomTemperatureReadingsByInterval('Sala', startDate, endDate);

      expect(mockModel.find).toHaveBeenCalledWith({
        room: 'Sala',
        timestamp: {
          $gte: new Date(startDate),
          $lte: new Date(endDate)
        }
      });
      expect(result).toBe(mockResult);
    });
  });

  describe('getTemperatureReadingById', () => {
    it('should return a reading by id', async () => {

      const mockPopulate = jest.fn().mockResolvedValue({ temperature: 30 });

      mockModel.findById.mockReturnValueOnce({
         populate: mockPopulate
      });

      const result = await service.getTemperatureReadingById('mockId');

      expect(mockModel.findById).toHaveBeenCalledWith('mockId');
      expect(mockPopulate).toHaveBeenCalledWith('room');
      expect(result).toEqual({ temperature: 30 });
    });

    it("should return an error if the id is invalid", async () => {
      await expect(service.getTemperatureReadingById(null)).rejects.toThrow("ID inválido");
    });

  });

  describe('deleteTemperatureReading', () => {
    it('should delete a reading by id', async () => {
      const deletedReading = { _id: 'mockId', temperature: 25 };
      mockModel.findByIdAndDelete.mockResolvedValueOnce(deletedReading);

      const result = await service.deleteTemperatureReading('mockId');

      expect(mockModel.findByIdAndDelete).toHaveBeenCalledWith('mockId');
      expect(result).toBe(deletedReading);
    });

    it("should return an error if the reading does not exist", async () => {
      mockModel.findByIdAndDelete.mockResolvedValueOnce(undefined);

      await expect(service.deleteTemperatureReading('mockId')).rejects.toThrow("Leitura não encontrada");
    });
  });
});
