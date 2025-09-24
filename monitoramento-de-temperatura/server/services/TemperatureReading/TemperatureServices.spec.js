import TemperatureService from "./TemperatureServices.js";
import { describe, jest } from '@jest/globals';

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

    it('should throw error if microcontrollerId is missing', async () => {
      const data = { temperature: 22, humidity: 50 };
      await expect(service.createTemperatureReading(data)).rejects.toThrow("MQTT: Dados incompletos recebidos.");
    });

    it('should throw error if temperature is missing', async () => {
      const data = { microcontrollerId: 'abc123', humidity: 50 };
      await expect(service.createTemperatureReading(data)).rejects.toThrow("MQTT: Dados incompletos recebidos.");
    });

    it('should throw error if humidity is missing', async () => {
      const data = { microcontrollerId: 'abc123', temperature: 22 };
      await expect(service.createTemperatureReading(data)).rejects.toThrow("MQTT: Dados incompletos recebidos.");
    });

    it('should throw error if temperature is not a number', async () => {
      const data = { microcontrollerId: 'abc123', temperature: "notANumber", humidity: 50 };
      await expect(service.createTemperatureReading(data)).rejects.toThrow("Temperatura e umidade devem ser números.");
    });

    it('should throw error if humidity is not a number', async () => {
      const data = { microcontrollerId: 'abc123', temperature: 22, humidity: "notANumber" };
      await expect(service.createTemperatureReading(data)).rejects.toThrow("Temperatura e umidade devem ser números.");
    });

    it('should throw error if room is not found', async () => {
      const data = { microcontrollerId: 'abc123', temperature: 22, humidity: 50 };
      mockRoomService.getRoomByMicrocontrollerId.mockResolvedValue(null);
      await expect(service.createTemperatureReading(data)).rejects.toThrow("MQTT: Nenhuma sala encontrada com microcontrolador abc123");
    });
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

    it('should throw error if roomId is missing', async () => {
      await expect(service.getRoomTemperatureReadings()).rejects.toThrow("ID da sala é obrigatório");
    });

  });

  describe('getTemperatureReadingById', () => {
    it('should throw error if reading not found', async () => {
      mockModel.findById.mockReturnValueOnce({
        populate: jest.fn().mockResolvedValue(null)
      });
      await expect(service.getTemperatureReadingById('mockId')).rejects.toThrow("Leitura não encontrada");
    });
  });

  describe('getTemperatureReadingsByInterval', () => {
    it('should return readings between two dates', async () => {
      const startDate = '2025-07-29T14:00:00Z';
      const endDate = '2025-07-30T14:00:00Z';
      const mockResult = ['reading in interval'];

      mockModel.find.mockReturnValueOnce({
        populate: jest.fn().mockResolvedValue(mockResult)
      });


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

    it('should throw error if roomId is missing', async () => {
      await expect(service.getRoomTemperatureReadingsByInterval()).rejects.toThrow("ID da sala é obrigatório");
    });

    it('should throw error if no readings found for room in interval', async () => {
      mockModel.find.mockReturnValueOnce({
        populate: jest.fn().mockResolvedValue([])
      });
      const startDate = '2025-07-29T00:00:00Z';
      const endDate = '2025-07-30T00:00:00Z';
      await expect(service.getRoomTemperatureReadingsByInterval('Sala', startDate, endDate)).rejects.toThrow("Nenhuma leitura encontrada para a sala no intervalo especificado");
    });

    it('should throw error if interval is invalid', async () => {
      const startDate = '2025-07-30T00:00:00Z';
      const endDate = '2025-07-29T00:00:00Z';
      await expect(service.getRoomTemperatureReadingsByInterval('Sala', startDate, endDate)).rejects.toThrow("Intervalo de datas inválido");
    });

  });

  describe('validateInterval', () => {
    it('should throw error if startDate is missing', async () => {
      await expect(service.validateInterval(null, '2025-07-30T00:00:00Z')).rejects.toThrow("Ambos os campos de data são obrigatórios");
    });

    it('should throw error if endDate is missing', async () => {
      await expect(service.validateInterval('2025-07-29T00:00:00Z', null)).rejects.toThrow("Ambos os campos de data são obrigatórios");
    });

    it('should throw error if dates are invalid', async () => {
      await expect(service.validateInterval('invalid', '2025-07-30T00:00:00Z')).rejects.toThrow("Datas inválidas");
      await expect(service.validateInterval('2025-07-29T00:00:00Z', 'invalid')).rejects.toThrow("Datas inválidas");
    });

    it('should throw error if startDate is after endDate', async () => {
      await expect(service.validateInterval('2025-07-30T00:00:00Z', '2025-07-29T00:00:00Z')).rejects.toThrow("Intervalo de datas inválido");
    });

    it('should throw error if endDate is in the future', async () => {
      const futureDate = new Date(Date.now() + 1000000).toISOString();
      await expect(service.validateInterval('2025-07-29T00:00:00Z', futureDate)).rejects.toThrow("Data final não pode ser no futuro");
    });

    it('should return true for valid interval', async () => {
      const startDate = '2022-01-01T00:00:00Z';
      const endDate = '2022-01-02T00:00:00Z';
      await expect(service.validateInterval(startDate, endDate)).resolves.toBe(true);
    });
  });

  describe('getRoomTemperatureReadings', () => {
    it('should return readings for a specific room', async () => {
      const mockResult = ['room reading'];
      mockModel.find.mockReturnValueOnce({
        populate: jest.fn().mockResolvedValue(mockResult)
      });

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

      mockModel.find.mockReturnValueOnce({
        populate: jest.fn().mockResolvedValue(mockResult)
      });

      const result = await service.getRoomTemperatureReadingsByInterval('Sala', startDate, endDate);
      const expectedStart = new Date(startDate);
      expectedStart.setHours(0, 0, 0, 0);
      const expectedEnd = new Date(endDate);
      expectedEnd.setHours(23, 59, 59, 999);
      expect(mockModel.find).toHaveBeenCalledWith({
        room: 'Sala',
        timestamp: {
          $gte: expectedStart,
          $lte: expectedEnd
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
