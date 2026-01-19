export function createBrazilRange(startDateStr, endDateStr) {
  const start = new Date(`${startDateStr}T00:00:00-03:00`);
  const end = new Date(`${endDateStr}T23:59:59.999-03:00`);
  return { start, end };
}