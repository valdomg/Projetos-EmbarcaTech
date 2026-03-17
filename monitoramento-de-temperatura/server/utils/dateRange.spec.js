import { createBrazilRange } from "./dateRange.js";

describe('createBrazilRange', () => {
  it('should create a date range with Brazil timezone', () => {
    const { start, end } = createBrazilRange('2025-01-15', '2025-01-20');
    expect(start).toBeInstanceOf(Date);
    expect(end).toBeInstanceOf(Date);
    expect(start < end).toBe(true);
  });

  it('start should be at 00:00:00 of the start date in Brazil timezone (-03:00)', () => {
    const { start } = createBrazilRange('2025-06-10', '2025-06-15');
    const expected = new Date('2025-06-10T00:00:00-03:00');
    expect(start.getTime()).toBe(expected.getTime());
  });

  it('end should be at 23:59:59.999 of the end date in Brazil timezone (-03:00)', () => {
    const { end } = createBrazilRange('2025-06-10', '2025-06-15');
    const expected = new Date('2025-06-15T23:59:59.999-03:00');
    expect(end.getTime()).toBe(expected.getTime());
  });
});
