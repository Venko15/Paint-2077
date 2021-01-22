#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

int REAL_X;
int REAL_Y;

#define NOT_DRAWN -1
#define DRAWN 1
#define DRAWN_BLUE 2
#define DRAWN_GREEN 3
#define DRAWN_RED 4
#define DRAWN_PURP 5
#define DRAWN_YELLOW 6
#define DRAWN_MAGENTA 7



#define TEMP_SZ 2000

typedef enum
{   BLACK=10,
    BLUE=20,
    GREEN=40,
    RED=70,
    PURP=80,
    YELLOW=90,
    MAGENTA=220
} clr;

typedef struct tagV2d
{
    double x, y;
} Vector2d;

#define VECTOR_RIGHT (Vector2d){1, 0}

Vector2d point_to_vector(POINT p)
{
    return (Vector2d)
    {
        (double)p.x, (double)p.y
    };
}
void clean_buffer(size_t lx, size_t ly, int arr[ly][lx])
{
    for(size_t i = 0; i < ly; i++)
        for(size_t j = 0; j < lx; j++)
            arr[i][j] = NOT_DRAWN;
}

POINT screen_to_console(POINT pt, int lx, int ly)
{
    return (POINT)
    {
        (((float)pt.x / REAL_X) * lx),
        (((float)pt.y / REAL_Y) * ly)
    };
}

double get_magnitude(Vector2d vc)
{
    return sqrt(vc.x * vc.x + vc.y * vc.y);
}

double dot_product(Vector2d v1, Vector2d v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

POINT *get_all_coords(int lx, int ly, POINT origin, POINT end)
{
    POINT org = screen_to_console(origin, lx, ly),
          ed = screen_to_console(end, lx, ly);

    POINT vector = {ed.x - org.x, ed.y - org.y};
    POINT *arr = malloc(TEMP_SZ * sizeof(POINT)), *init_mem = arr;

    Vector2d resulting_vc = point_to_vector(vector);

    double res_magnitude = get_magnitude(resulting_vc);
    double cosAngle = dot_product(resulting_vc, VECTOR_RIGHT) / res_magnitude,
           sinAngle = sqrt(1 - cosAngle * cosAngle);

    sinAngle = vector.y < 0 ? -sinAngle : sinAngle;


    for(double i = 0; i <= res_magnitude; i++)
    {
        LONG _x = (LONG)(cosAngle * i) + (LONG)org.x;
        LONG _y = (LONG)(sinAngle * i) + (LONG)org.y;
        *arr++ = (POINT)
        {
            _x, _y
        };

    }

    *arr = (POINT)
    {
        REAL_X + 10, REAL_Y + 10
    };
    return init_mem;
}

void draw_line(int lx, int ly,POINT firstPos, WORD col, HANDLE cons,int drawnPoints[][ly])
{
    POINT secondPos = {REAL_X + 10, REAL_Y + 10};
    DWORD dwrd;

    Sleep(100);
    while(1)
    {
        if(GetAsyncKeyState(VK_RIGHT))
        {
            GetCursorPos(&secondPos);
            break;
        }

        if(GetAsyncKeyState(VK_UP))
            return;
    }

    POINT *coords = get_all_coords(lx, ly, firstPos, secondPos);

    for(POINT *c = coords; c->x != REAL_X + 10; c++)
    {
        drawnPoints[c->x][c->y]=col;
        FillConsoleOutputAttribute(cons, col, 1, (COORD)
        {
            c->x, c->y
        }, &dwrd);

    }

    free(coords);
}



void get_colour(WORD *colour)
{

    if(GetAsyncKeyState(VK_SPACE))
    {
        *colour=250;

    }
    else
    {
        *colour=BLACK;
    }
    if(GetAsyncKeyState(VK_F1))
    {
        *colour=BLUE;

    }
    else if(GetAsyncKeyState(VK_F2))
    {
        *colour = GREEN;

    }
    else if(GetAsyncKeyState(VK_F3))
    {
        *colour = RED;

    }
    else if(GetAsyncKeyState(VK_F4))
    {
        *colour = PURP;

    }
    else if(GetAsyncKeyState(VK_F5))
    {
        *colour = YELLOW;

    }



}
void get_brush_size(int *brush_size)
{
    if(GetAsyncKeyState(VK_NUMPAD1))
    {
        *brush_size=1;

    }
    else if(GetAsyncKeyState(VK_NUMPAD2))
    {
        *brush_size=2;

    }
    else if(GetAsyncKeyState(VK_NUMPAD3))
    {
        *brush_size=3;

    }
    else if(GetAsyncKeyState(VK_NUMPAD4))
    {
        *brush_size=4;

    }
    else if(GetAsyncKeyState(VK_NUMPAD5))
    {
        *brush_size=5;

    }
    else if(GetAsyncKeyState(VK_NUMPAD6))
    {
        *brush_size=6;

    }
    else if(GetAsyncKeyState(VK_NUMPAD0))
    {
        *brush_size=100;

    }




}
void draw_rectangle(int lx, int ly,POINT firstPos, WORD col, HANDLE cons,int *drawnPoints[][ly])
{
    POINT secondPos;
    DWORD dwrd;
    Sleep(100);
    POINT fp=screen_to_console(firstPos,lx,ly);

    while(1)
    {
        if(GetAsyncKeyState(VK_LEFT))
        {
            GetCursorPos(&secondPos);

            break;
        }

        if(GetAsyncKeyState(VK_UP))
            return;
    }

    POINT sp=screen_to_console(secondPos,lx,ly);
    if(fp.x>sp.x && fp.y>sp.y)
    {
        POINT temp = sp;
        sp = fp;
        fp = temp;


    }
    else if(fp.x>sp.x)
    {
        int temp = sp.x;
        sp.x = fp.x;
        fp.x = temp;

    }
    if(fp.y>sp.y )
    {
        int temp = sp.y;
        sp.y = fp.y;
        fp.y = temp;

    }


    for(LONG y = fp.y; y<=sp.y; y++)
    {

        for(LONG x = fp.x; x<=sp.x; x++)
        {
            if(y==sp.y || x==fp.x || x==sp.x || y==fp.y)
            {

                drawnPoints[x][y]=col;
                FillConsoleOutputAttribute(cons, col, 1, (COORD)
                {
                    x, y
                }, &dwrd);
            }
        }

    }


}
void fill(int lx, int ly,POINT pos, WORD col, HANDLE cons,int drawnPoints[][ly])
{

     if(drawnPoints[pos.x+1][pos.y] != NOT_DRAWN && drawnPoints[pos.x-1][pos.y] != NOT_DRAWN || drawnPoints[pos.x][pos.y+1] != NOT_DRAWN && drawnPoints[pos.x][pos.y-1] != NOT_DRAWN) return;

  DWORD dwrd;
  POINT coords[] = {{pos.x, pos.y},{pos.x + 1, pos.y}, {pos.x - 1, pos.y}, {pos.x, pos.y - 1}, {pos.x, pos.y + 1}};//masiv ot susedni tochki na natisnatata
  for(int i = 0; i < 5; i++) {

   FillConsoleOutputAttribute(cons, col, 1, (COORD){coords[i].x,coords[i].y}, &dwrd);

    drawnPoints[coords[i].x][coords[i].y] = col;

if(i!=0){

    fill(lx, ly, coords[i], col,cons, drawnPoints);
}

  }

}

int main()
{
    printf("if you want a new file, enter a new name other than the existing ones(it needs to end with .ppm at the end)\n File name:");
    char filename[100];
    scanf("%s", filename);
    POINT pos;
    WORD colour=10;
    HANDLE cons=GetStdHandle(STD_OUTPUT_HANDLE);
    system("mode 700");
    DWORD dwrd;
    CONSOLE_SCREEN_BUFFER_INFO a;
    GetConsoleScreenBufferInfo(cons, &a);
    size_t lenX = a.dwMaximumWindowSize.X, lenY = a.dwMaximumWindowSize.Y;
    int drawnPoints[lenX][lenY];
    int brush_size=1;
   REAL_X=GetSystemMetrics(SM_CXSCREEN);
     REAL_Y=GetSystemMetrics(SM_CYSCREEN);
    clean_buffer(lenX, lenY, drawnPoints);



    for(LONG y = 0; y<lenY; y++)
    {

        for(LONG x = 0; x<lenX;x++)
        {
            if(y==lenY || x==0 || x==lenX || y==0)
            {

                drawnPoints[x][y]=BLACK;

            }
        }

    }





    while(TRUE)
    {
        GetCursorPos(&pos);
        get_colour(&colour);
        get_brush_size(&brush_size);

        if(GetAsyncKeyState(VK_RBUTTON))
        {
            COORD crd = {((float)pos.x / REAL_X) * a.dwMaximumWindowSize.X,
                         ((float)pos.y / REAL_Y) * a.dwMaximumWindowSize.Y
                        };

            if( colour!=250)
            {
                if( brush_size==1)
                {
                     drawnPoints[crd.X][crd.Y]  = colour;
                }
                else if( brush_size>1)
                {

                    for(int i=0; i< brush_size; i++)
                    {


                        drawnPoints[crd.X+i][crd.Y]  = colour;
                    }

                }
            }
            else
            {

                drawnPoints[crd.X][crd.Y] = NOT_DRAWN;
                if(brush_size>1)
                {
                    for(int i=0; i<brush_size; i++)
                    {
                        drawnPoints[crd.X+i][crd.Y] = NOT_DRAWN;
                    }

                }
            }

            FillConsoleOutputAttribute(cons, colour, brush_size, crd, &dwrd);

        }
        else if(GetAsyncKeyState(VK_RIGHT))
        {
            draw_line(lenX, lenY, pos, colour, cons,&drawnPoints);
        }
        else if(GetAsyncKeyState(VK_LEFT))
        {
            draw_rectangle(lenX, lenY, pos, colour, cons,&drawnPoints);


        }else if(GetAsyncKeyState(VK_DOWN))
        {

            fill( lenX,  lenY, screen_to_console(pos,lenX,lenY),  colour,  cons, &drawnPoints);


        }

       else if(GetAsyncKeyState(VK_RETURN))
        {
            FILE *img = fopen(filename, "w+");

            fprintf(img, "P3\n");
            fprintf(img, "%d %d\n", (int)(lenX), (int)(lenY));
            fprintf(img, "255\n");

            for(size_t i = 0; i < lenY; i++)
            {
                for(size_t j = 0; j < lenX; j++)
                {
                    if(drawnPoints[j][i] != NOT_DRAWN )
                    {
                        if(drawnPoints[j][i] == BLACK)
                        {
                            fprintf(img, "%d %d %d\n", 0, 0, 0);
                        }
                        else if(drawnPoints[j][i] == BLUE)
                        {
                            fprintf(img, "%d %d %d\n", 0, 0, 255);


                        }
                        else if(drawnPoints[j][i] == GREEN)
                        {
                            fprintf(img, "%d %d %d\n", 0, 255, 0);


                        }
                        else if(drawnPoints[j][i] == RED)
                        {
                            fprintf(img, "%d %d %d\n", 255, 0, 0);


                        }
                        else if(drawnPoints[j][i] == PURP)
                        {
                            fprintf(img, "%d %d %d\n",128,0,128);


                        }

                    }
                    else
                    {
                        fprintf(img, "%d %d %d\n", 255, 255, 255);
                    }

                }
            }

            fclose(img);
        }

    }
    CloseHandle(cons);

    return 0;
}

