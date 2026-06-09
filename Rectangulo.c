#include<stdio.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_primitives.h>

int main()
{
    float a1,b1,c1,d1,a2,b2,c2,d2;

    al_init();
    al_install_keyboard();
    al_init_primitives_addon();

    ALLEGRO_DISPLAY *display=al_create_display(1000,1000);
    ALLEGRO_EVENT_QUEUE *queue=al_create_event_queue();

    al_register_event_source(queue,al_get_keyboard_event_source());

    float x1=700,x2=750,y1=400,y2=450,a=1;
    float x3=0,x4=1000,y3=200,y4=250;
    float x5=0,x6=1000,y5=500,y6=550;
    float x7=0,x8=50,y7=200,y8=550;
    float x9=950,x10=1000,y9=200,y10=550;
    float pilar1=600,pilar2=650,pilar3=300,pilar4=500;

    while(a)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if(ev.type==ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(ev.keyboard.keycode)
            {
                case ALLEGRO_KEY_UP:
                    y1-=50; y2-=50;
                    if(y1==y3 || y2==y4 || y1==y5 || y2==y6 || y1==y7 || y2==y8 || (y1==pilar3 && (x1>=pilar1 && x2<=pilar2)) || (y2==pilar4 && (x1>=pilar1 && x2<=pilar2)))
                    {
                        y1+=50; y2+=50;
                    }
                    break;
                case ALLEGRO_KEY_DOWN:
                    y1+=50; y2+=50;
                    if(y1==y3 || y2==y4 || y1==y5 || y2==y6 || y1==y7 || y2==y8 || (y1==pilar3 && (x1>=pilar1 && x2<=pilar2)) || (y2==pilar4 && (x1>=pilar1 && x2<=pilar2)))
                    {
                        y1-=50; y2-=50;
                    }
                    break;
                case ALLEGRO_KEY_LEFT:
                    x1-=50; x2-=50;
                    if(x1==x3 || x2==x4 || x1==x5 || x2==x6 || x1==x7 || x2==x8 || (x1==pilar1 && (y1>=pilar3 && y2<=pilar4)) || (x2==pilar2 && (y1>=pilar3 && y2<=pilar4)))
                    {
                        x1+=50; x2+=50;
                    }
                    break;
                case ALLEGRO_KEY_RIGHT:
                    x1+=50; x2+=50;
                    if(x1==x3 || x2==x4 || x1==x5 || x2==x6 || x1==x7 || x2==x8 || (x1==pilar1 && (y1>=pilar3 && y2<=pilar4)) || (x2==pilar2 && (y1>=pilar3 && y2<=pilar4)))
                    {
                        x1-=50; x2-=50;
                    }
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    a=0;
                    break;
            }
        }
        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_rectangle(x1,y1,x2,y2, al_map_rgb(0,150,255),1);
        al_draw_rectangle(x3,y3,x4,y4, al_map_rgb(100,23,255),3);
        al_draw_rectangle(x5,y5,x6,y6, al_map_rgb(100,23,255),3);
        al_draw_rectangle(x7,y7,x8,y8, al_map_rgb(100,23,255),3);
        al_draw_rectangle(x9,y9,x10,y10, al_map_rgb(100,23,255),3);
        al_draw_rectangle(pilar1,pilar3,pilar2,pilar4, al_map_rgb(255,50,55),3);
        al_flip_display();
    }

    al_destroy_display(display);
    al_destroy_event_queue(queue);

    return 0;
}
//gcc Rectangulo.c -o Rectangulo $(pkg-config --libs --cflags allegro-5 allegro_main-5 allegro_primitives-5)