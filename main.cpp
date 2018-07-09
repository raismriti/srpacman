#include <time.h>
#include <graphics.h>
#include <windows.h>

/*****************************************************************************
* a cell is a 5 pixel x 5 pixel square.
* the top 4 x left 4 is the cell with the bottom row and right side being walls
*****************************************************************************/
#define PER_CELL 15
#define X_MIN    0
#define X_MAX    639
#define X_CELLS 40
#define Y_MIN    0
#define Y_MAX    479
#define Y_CELLS 30
#define X_OFF 25
#define Y_OFF 25
#define SPEED 3
#define PM_RADIUS 8
#define G_RADIUS 6
// Function to draw a filled bar using the fillpoly function in BGI
void box(int x, int y, int xx, int yy, int c);
void box_cell (int x, int y, int c);
void draw_pacman(int pm_x, int pm_y, int mouth, int direction);
void draw_ghost(int g_x, int g_y, int color, int direction);

int main()
{

    // Variables needed for the BGI library initialization
    int gd=DETECT;
    int gm;

    int direction = 0;
    int ndirection = 0;
    int pm_x=X_OFF, pm_y=Y_OFF;
    int g_x=X_OFF + 10, g_y=Y_OFF;
    int dots[X_CELLS][Y_CELLS];
    int x, y, z=0;
    int a;
    int dx=0, dy=0;
    int ndx=0, ndy=0;
    int mouth;
    int score = 0;
    int delay = 8;
    int cellx;
    int celly;
    int points [8];
    int green_ghost [2] = {((X_CELLS/2) * PER_CELL + X_OFF), ((Y_CELLS/2) * PER_CELL + X_OFF)};
    int red_ghost [2] = {((X_CELLS/2 - 1) * PER_CELL + X_OFF), ((Y_CELLS/2) * PER_CELL + X_OFF)};
    int yellow_ghost [2] {((X_CELLS/2 + 1)* PER_CELL + X_OFF), ((Y_CELLS/2) * PER_CELL + X_OFF)};
    int pink_ghost [2] {((X_CELLS/2) * PER_CELL + X_OFF), ((Y_CELLS/2 - 1) * PER_CELL + X_OFF)};


    initgraph(&gd, &gm, "C:\\TC\\BGI");
    srand(time(NULL));

    // Loop for new maze
    while(TRUE){

        // Draw Maze Box
        // Clear the screen
        box(0,0,639,479,BLACK);
        // Fill usable area with neutral color
//        box(X_MIN,Y_MIN,X_MAX,Y_MAX,DARKGRAY);
        // Draw a border
        setcolor(BLUE);
        rectangle(X_MIN,Y_MIN,X_MAX,Y_MAX);
        rectangle(X_MIN+1,Y_MIN+1,X_MAX-1,Y_MAX-1);

        setcolor(LIGHTRED);
        setfillstyle(SOLID_FILL, LIGHTRED);
        for(x=0;x<X_CELLS;x++)
            for(y=0;y<Y_CELLS;y++)
                fillellipse(x*15+X_OFF, y*15+Y_OFF, 2, 2);
                dots[x][y]=1;
        for (a=1;a<3;a++){
            box_cell (X_CELLS/2 - a, Y_CELLS/2 - 1, WHITE);
            box_cell (X_CELLS/2 + a, Y_CELLS/2 - 1, WHITE);
        }

            box_cell (X_CELLS/2 - 2, Y_CELLS/2, WHITE);
            box_cell (X_CELLS/2 + 2, Y_CELLS/2, WHITE);
        for (a= -2;a<3;a++){
            box_cell (X_CELLS/2 - a, Y_CELLS/2 + 1, WHITE);
        }

        while(z==0) {
            setcolor(BLACK);
            setfillstyle(SOLID_FILL, BLACK);
            fillellipse(pm_x, pm_y, PM_RADIUS, PM_RADIUS);
            /*points[0] = g_x - (G_RADIUS + 1);
            points[1] = g_y - G_RADIUS;
            points[2] = g_x - (G_RADIUS + 1);
            points[3] = g_y + G_RADIUS ;
            points[4] =  g_x + (G_RADIUS + 1);
            points[5] = g_y + G_RADIUS;
            points[6] = g_x + (G_RADIUS + 1);
            points[7] = g_y - G_RADIUS;
            fillpoly(4, points); */
            mouth++;
            if( mouth > 7)
                mouth = 0;
            pm_x += dx;
            pm_y += dy;
            if (((pm_x - X_OFF)%15 == 0) && ((pm_y - Y_OFF)%15 == 0)){
                dx = ndx;
                dy = ndy;
                direction = ndirection;
                cellx = (pm_x - X_OFF)/15;
                celly = (pm_y - Y_OFF)/15;
                score = score + 10 * dots[cellx][celly];
                dots[cellx][celly] = 0;
                if(cellx==0 && direction==2){
                    dx = 0;
                    ndx = 0;
                }
                if(cellx==(X_CELLS-1) && direction==0){
                    dx = 0;
                    ndx = 0;
                }
                if(celly==0 && direction==1){
                    dy = 0;
                    ndy = 0;
                }
                if(celly==(Y_CELLS-1) && direction==3){
                    dy = 0;
                    ndy = 0;
                }
            }
            draw_pacman(pm_x, pm_y, mouth, direction);
            draw_ghost(green_ghost[0],green_ghost [1], GREEN, direction);
            draw_ghost(red_ghost[0], red_ghost [1], RED, direction);
            draw_ghost(yellow_ghost[0], yellow_ghost [1], YELLOW, direction);
            draw_ghost(pink_ghost[0], pink_ghost [1], LIGHTRED, direction);

            if (kbhit()) {
                switch(getch()){
                case KEY_RIGHT:
                    ndx = SPEED;
                    ndy = 0;
                    ndirection = 0;
                    break;
                case KEY_LEFT:
                    ndx = -1 * SPEED;
                    ndy = 0;
                    ndirection = 2;
                    break;
                case KEY_DOWN:
                    ndx = 0;
                    ndy = SPEED;
                    ndirection = 3;
                    break;
                case KEY_UP:
                    ndx = 0;
                    ndy = -1 * SPEED;
                    ndirection = 1;
                    break;
                case KEY_F1:
                    delay -= 10;
                    if (delay < 0)
                        delay = 0;
                    break;
                case KEY_F2:
                    delay += 10;
                    break;
                default:
                    ndx = 0;
                    ndy = 0;
                    break;
                }
            }
            Sleep(delay);
        }
    }
}
// box subroutine
// Initializes the poly array holding the x and y coordinates of the rectangle corners
// and calls the fillpoly BGI routine to dray and fill the polynomial
void box(int x, int y, int xx, int yy, int c)
{
    int poly[10];
    setcolor(c);
    setfillstyle(SOLID_FILL, c);
    poly[0]=poly[8]=poly[6]=x;
    poly[1]=poly[9]=poly[3]=y;
    poly[2]=poly[4]=xx;
    poly[5]=poly[7]=yy;
    fillpoly(5, poly);

}
void box_cell (int x, int y, int color) {
    setcolor(color);
    setfillstyle(SOLID_FILL, BLACK);
    bar ((x * PER_CELL) - 7 + X_OFF,(y * PER_CELL) - 7 + Y_OFF, (x * PER_CELL) + 7 + X_OFF,(y * PER_CELL) + 7 + Y_OFF);
    rectangle((x * PER_CELL) - 7 + X_OFF,(y * PER_CELL) - 7 + Y_OFF, (x * PER_CELL) + 7 + X_OFF,(y * PER_CELL) + 7 + Y_OFF);


}
void draw_pacman(int pm_x, int pm_y, int mouth, int direction){
    int points[6];
    int e_x=0;
    int e_y=0;

    points[0] = pm_x;
    points[1] = pm_y;
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse(pm_x, pm_y, PM_RADIUS, PM_RADIUS);
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);
    switch (direction) {
    case 0:
        points[2] = pm_x+(PM_RADIUS);
        points[3] = pm_y-(PM_RADIUS);
        points[4] = pm_x+(PM_RADIUS);
        points[5] = pm_y+(PM_RADIUS);
        e_x = pm_x;
        e_y = pm_y - 4;
        break;
    case 1:
        points[2] = pm_x+(PM_RADIUS);
        points[3] = pm_y-(PM_RADIUS);
        points[4] = pm_x-(PM_RADIUS);
        points[5] = pm_y-(PM_RADIUS);
        e_x = pm_x - 4;
        e_y = pm_y;
        break;
    case 2:
        points[2] = pm_x-(PM_RADIUS);
        points[3] = pm_y+(PM_RADIUS);
        points[4] = pm_x-(PM_RADIUS);
        points[5] = pm_y-(PM_RADIUS);
        e_x = pm_x;
        e_y = pm_y - 4;
        break;
    case 3:
        points[2] = pm_x+(PM_RADIUS);
        points[3] = pm_y+(PM_RADIUS);
        points[4] = pm_x-(PM_RADIUS);
        points[5] = pm_y+(PM_RADIUS);
        e_x = pm_x - 4;
        e_y = pm_y;
        break;
    }
    if (mouth > 3)
        fillpoly(3, points);
    circle(e_x, e_y, 1);
}

void draw_ghost(int g_x, int g_y, int color, int direction){
    int points[8];

    points[0] = g_x - G_RADIUS;
    points[1] = g_y;
    points[2] = g_x - (G_RADIUS + 1);
    points[3] = g_y + G_RADIUS ;
    points[4] =  g_x + (G_RADIUS + 1);
    points[5] = g_y + G_RADIUS;
    points[6] = g_x + G_RADIUS;
    points[7] = g_y;
    setcolor(color);
    setfillstyle(SOLID_FILL, color);
    fillellipse(g_x, g_y, G_RADIUS, G_RADIUS);
    fillpoly(4, points);
    setcolor(BLACK);
    circle(g_x - 2, g_y - 1, 1);
    circle(g_x + 2, g_y - 1, 1);

}
