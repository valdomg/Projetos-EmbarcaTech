import TemperatureService from "./TemperatureServices.js";
import { jest } from '@jest/globals';

const mockModel = {
  find: jest.fn(),
  findById: jest.fn(),
  findByIdAndDelete: jest.fn(),
  create: jest.fn(),
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

      const result = await service.createTemperatureReading(data);

      expect(saveMock).toHaveBeenCalled();
      expect(result).toBe('mocked saved reading');
    });
  });

  describe('getTemperatureReadings', () => {
    it('should fetch all readings', async () => {
      mockModel.find.mockResolvedValueOnce(['reading1', 'reading2']);

      const result = await service.getTemperatureReadings();

      expect(mockModel.find).toHaveBeenCalledWith();
      expect(result).toEqual(['reading1', 'reading2']);
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
  });

  describe('getRoomTemperatureReadings', () => {
    it('should return readings for a specific room', async () => {
      const mockResult = ['room reading'];
      mockModel.find.mockResolvedValueOnce(mockResult);

      const result = await service.getRoomTemperatureReadings('Sala');

      expect(mockModel.find).toHaveBeenCalledWith({ room: 'Sala' });
      expect(result).toBe(mockResult);
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
      const mockReading = { temperature: 30 };
      mockModel.findById.mockResolvedValueOnce(mockReading);

      const result = await service.getTemperatureReadingById('mockId');

      expect(mockModel.findById).toHaveBeenCalledWith('mockId');
      expect(result).toBe(mockReading);
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
  });
});
