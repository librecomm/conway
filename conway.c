#define XOPEN_SOURCE 700

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define LEFT(x)     (x-1)    >= 0         && (x-1) % COLS < (x) % COLS
#define RIGHT(x)    (x+1)    <= COLS*ROWS && (x+1) % COLS > (x) % COLS
#define UP(x)       (x-COLS) >= 0
#define DOWN(x)     (x+COLS) <= COLS*ROWS
#define DRAW(b,c,r) if (r*COLS+c <= COLS*ROWS)\
				   b[r*COLS+c] = 1;

/* config section */
#define COLS 80
#define ROWS 25
#define GPS 60

int  in(int cells[COLS*ROWS], char *path);
int  adj(int cells[COLS*ROWS], size_t cell);
void gen(int cells[COLS*ROWS]);
void render(int cells[COLS*ROWS]);

int
main(int argc, char **argv)
{
	int cells[COLS*ROWS] = { 0 };

	if (argc != 2) {
		(void)write(1, "usage: conway [file]\n", 21);
		return 1;
	}

	if (in(cells, argv[1]) == -1)
		return 1;

	while (1) {
		render(cells);
		gen(cells);
		usleep(1000000/GPS);
	}

	return 0;
}

int
in(int cells[COLS*ROWS], char *path)
{
	int fd;
	char buf[COLS*ROWS];
	ssize_t buf_size;

	if ((fd = open(path, O_RDONLY)) == -1)
		return -1;

	if ((buf_size = read(fd, buf, COLS*ROWS)) == -1)
		return -1;

	size_t row = 0;
	size_t col = 0;

	for (size_t i = 0; i < buf_size; i++) {
		if (buf[i] == '\n') {
			col = 0;
			row++;
		}

		if (buf[i] != ' ' && buf[i] != '\n')
			DRAW(cells,col,row);

		col++;
	}

	return 0;
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
