float half(float x1, float x2, float m1, float m2) {
	return ((x1 * m1) + (x2 * m2)) / (m1 + m2);
}

void dock(Ship* ship, Ship* station) {
	ship->docked_at = station;
	station->cx = half(station->cx, ship->cx, station->mass, ship->mass);
	station->cy = half(station->cy, ship->cy, station->mass, ship->mass);

	ship->cx = half(station->cx, ship->cx, station->mass, ship->mass);
	ship->cy = half(station->cy, ship->cy, station->mass, ship->mass);

	station->pos.da = half(station->pos.da, ship->pos.da, station->mass, ship->mass);
	station->pos.dx = half(station->pos.dx, ship->pos.dx, station->mass, ship->mass);
	station->pos.dy = half(station->pos.dy, ship->pos.dy, station->mass, ship->mass);

	ship->pos.da = half(station->pos.da, ship->pos.da, station->mass, ship->mass);
	ship->pos.dx = half(station->pos.dx, ship->pos.dx, station->mass, ship->mass);
	ship->pos.dy = half(station->pos.dy, ship->pos.dy, station->mass, ship->mass);
}