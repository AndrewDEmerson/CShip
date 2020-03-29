#include <stdio.h>
#include <unistd.h>
#include <curses.h>
int h, w;
int cy = 0, cx = 5;
int gridy = 0, gridx = 0;
int gridScale = 4;
int grid_dy = 10, grid_dx = 10;

int findIndex();

int main(int arcc, char **argv)
{
    initscr();            //initalize ncurses
    cbreak();             //disable tty buffer
    noecho();             //do not echo tty input into terminal
    keypad(stdscr, TRUE); //enable special keys
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLUE);
    init_pair(2, COLOR_MAGENTA, COLOR_BLUE);

    getmaxyx(stdscr, h, w); //get screen dimensions

    WINDOW *egrid = newwin(gridy + grid_dy * gridScale + 1, gridx + grid_dx * gridScale + 1, gridy, gridx);
    WINDOW *buglog = newwin(1, w - 1, h - 1, 0);
    waddstr(buglog, "use vim keys to move, f to fire, Q to quit");
    wrefresh(buglog);

    while (1)
    {
        for (int y = 0; y < (grid_dy * gridScale + 1); y++)
        {
            for (int x = 0; x < (grid_dx * gridScale + 1); x++)
            {
                wmove(egrid, y, x);
                wattron(egrid, COLOR_PAIR(2));
                if (y % gridScale == 0 && x % gridScale == 0)
                {
                    waddch(egrid, '+');
                    continue;
                }
                if (y % gridScale == 0)
                {
                    waddch(egrid, '-');
                    continue;
                }
                if (x % gridScale == 0)
                {
                    waddch(egrid, '|');
                    continue;
                }
                wattroff(egrid, COLOR_PAIR(2));
                waddch(egrid, '~' | COLOR_PAIR(1) | A_BOLD);
            }
        }
        wmove(egrid, cy, cx);
        wrefresh(egrid);

        int key = wgetch(egrid);
        if (key == 'Q')
        {
            break;
        }
        switch (key)
        {
        case 'h':
            if (cx > 0)
            {
                cx--;
            }
            break;
        case 'j':
            if (cy < grid_dy * gridScale)
            {
                cy++;
            }
            break;
        case 'k':
            if (cy > 0)
            {
                cy--;
            }
            break;
        case 'l':
            if (cx < grid_dx * gridScale)
            {
                cx++;
            }
            break;
        case 'y':
            if (cy > 0 && cx > 0)
            {
                cy--;
                cx--;
            }
            break;
        case 'u':
            if (cy > 0 && cx < grid_dx * gridScale)
            {
                cy--;
                cx++;
            }
            break;
        case 'b':
            if (cy < grid_dy * gridScale && cx > 0)
            {
                cy++;
                cx--;
            }
            break;
        case 'n':
            if (cy < grid_dy * gridScale && cx < grid_dx * gridScale)
            {
                cy++;
                cx++;
            }
            break;
        case 'f':
            wmove(buglog, 0, 0);
            char tmp[100];
            sprintf(tmp, "tui cords: y: %i,\tx: %i\t\tindex: %3i", cy, cx, findIndex());
            waddstr(buglog, tmp);
            wrefresh(buglog);
            //_exit;
            //printf("y: %i,\tx: %i\n", cy, cx);
            break;
        default:
            break;
        }
    }
    endwin();
}

int findIndex()
{
    if (cx % gridScale == 0 || cy % gridScale == 0)
    {
        return -1;
    }
    int tv = cx / gridScale;
    int ty = cy / gridScale;
    int tt = tv + grid_dx * ty;
    return tt;
}