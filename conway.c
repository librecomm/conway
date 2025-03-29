#define XOPEN_SOURCE 700

#include <stdlib.h>
#include <unistd.h>

#define LEFT(x)  (x-1)    >= 0         && (x-1) % COLS < (x) % COLS
#define RIGHT(x) (x+1)    <= COLS*ROWS && (x+1) % COLS > (x) % COLS
#define UP(x)    (x-COLS) >= 0
#define DOWN(x)  (x+COLS) <= COLS*ROWS
#define XY(x,y)  (y)*COLS+(x)

#define COLS 80
#define ROWS 25
#define GPS 60

int  adj(int cells[COLS*ROWS], size_t cell);
void gen(int cells[COLS*ROWS]);
void render(int cells[COLS*ROWS]);

int
main(int argc, char **argv)
{
	int cells[COLS*ROWS] = { 0 };

	cells[XY(40,15)] = 1;
	cells[XY(40,16)] = 1;
	cells[XY(40,17)] = 1;
	cells[XY(41,15)] = 1;
	cells[XY(42,15)] = 1;
	cells[XY(42,16)] = 1;
	cells[XY(42,17)] = 1;

	while (1) {
		render(cells);
		gen(cells);
		usleep(1000000/GPS);
	}
}

int
adj(int cells[COLS*ROWS], size_t i)
{
	int c = 0;
	
	if (cells[i-1] == 1      && LEFT(i))  c++;
	if (cells[i+1] == 1      && RIGHT(i)) c++;
	if (cells[i-COLS] == 1   && UP(i))    c++;
	if (cells[i+COLS] == 1   && DOWN(i))  c++;
	if (cells[i-COLS-1] == 1 && UP(i-1)   && LEFT(i-COLS))  c++;
	if (cells[i-COLS+1] == 1 && UP(i+1)   && RIGHT(i-COLS)) c++;
	if (cells[i+COLS-1] == 1 && DOWN(i-1) && LEFT(i+COLS))  c++;
	if (cells[i+COLS+1] == 1 && DOWN(i+1) && RIGHT(i+COLS)) c++;

	return c;
}

void
gen(int cells[COLS*ROWS])
{
	int new[COLS*ROWS] = { 0 };

	for (size_t i = 0; i < COLS*ROWS; i++) {
		new[i] = cells[i];

		if(cells[i] == 0 && adj(cells, i) == 3)
			new[i] = 1;

		if(cells[i] == 1 && (adj(cells, i) < 2 || adj(cells, i) > 3))
			new[i] = 0;
	}

	for(size_t i = 0; i < COLS*ROWS; i++)
		cells[i] = new[i];
}

void
render(int cells[COLS*ROWS])
{
	char buf[(COLS+1)*ROWS];

	size_t j = 0;

	(void)write(1, "\e[2J\e[H", 7);

	for(size_t i = 0; i < COLS*ROWS; i++) {
		if (i % COLS == 0 && i != 0) {
			buf[i+j] = '\n';
			j++;
		}

		buf[i+j] = ((cells[i] == 1) ? '#' : ' ');
	}

	(void)write(1, buf, (COLS+1)*ROWS);
}
