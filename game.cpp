#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <chrono>
#include <thread>

#define F first
#define S second
#define HEIGHT 1000
#define WIDTH 1000
#define PANEL_WIDTH 150
#define N 20
#define CELL_COLOR Color{255, 255, 255, 255}
#define SELECTED_COLOR Color{100, 200, 255, 255}

#ifdef main
#undef main
#endif

using namespace std;

struct Color{
    int r;
    int g;
    int b;
    int a;
};

class Button{
public:
    Button(){
        size = {0, 0};
    }
    Button(pair<int, int> _pos, pair<int, int> _size, Color _color){
        pos = _pos;
        size = _size;
        color = _color;
        rect = {pos.F, pos.S, size.F, size.S}; 
    }
    
    void draw(SDL_Renderer *renderer){
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
    }

    bool is_Clicked(pair<int, int> _pos){
        if (_pos.F > pos.F && _pos.F < (pos.F + size.F) && _pos.S > pos.S && _pos.S < pos.S + size.S)
            return true;
        else
            return false;
    }

    pair<int, int> pos;
    pair<int, int> size;
    Color color; 
    SDL_Rect rect; 
};

class GOL{
private:
    SDL_Window *window;
    SDL_Renderer* rend;
    SDL_Event event;
    int cnt;
    bool table[N][N];
    bool answer[N][N];
    Button *button_table[N][N];
    TTF_Font * font;
    SDL_Color text_color;
    SDL_Surface* image_sur[6];
    Button start;
    Button stop;
    Button clear;
    Button up;
    Button down;
    Button upload;
    int speed;
    string file_name;

public:
    GOL(bool table_in[N][N], string name){
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
            printf("Error initializing SDL: %s\n", SDL_GetError());
            return;
        }
        window = SDL_CreateWindow("GOL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH + PANEL_WIDTH, HEIGHT, 0);
        if (!window){
            printf("Error creating window: %s\n", SDL_GetError());
            SDL_Quit();
            return;
        }
        Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
        rend = SDL_CreateRenderer(window, -1, render_flags);
        if (!rend){
            printf("Error creating renderer: %s\n", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();
            return;
        }

        if (TTF_Init() < 0 ) {
            cout << "Error initializing SDL_ttf: " << TTF_GetError() << endl;
            SDL_Quit();
            return;
        }

        if (IMG_Init(IMG_INIT_PNG) == 0) {
            std::cout << "Error SDL2_image Initialization";
            return;
        }

        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                button_table[i][j] = new Button({i * (WIDTH / N), j * (HEIGHT / N)}, {WIDTH / N, HEIGHT / N}, CELL_COLOR);
                table[i][j] = false;
            }
        }

        // buttons for start
        start = Button({WIDTH, HEIGHT / 2 - 80}, {100, 100}, Color{0, 0, 0, 0});
        stop = Button({WIDTH, 0}, {100, 150}, Color{0, 0, 0, 0});
        clear = Button({WIDTH, HEIGHT - 150}, {100, 150}, Color{0, 0, 0, 0});
        up = Button({WIDTH, HEIGHT / 2 + 50}, {50, 50}, Color{0, 0, 0, 0});
        down = Button({WIDTH, HEIGHT / 2 + 120}, {50, 50}, Color{0, 0, 0, 0});
        upload = Button({WIDTH, HEIGHT * 3 / 4 - 50}, {80, 80}, Color{0, 0, 0, 0});
        
        //loading font
        font = TTF_OpenFont("Arial.ttf", 25);
        text_color = SDL_Color{0, 0, 0};
        cnt = 0;
        speed = 1;

        //loading image
        image_sur[0] = IMG_Load("start.png");
        image_sur[1] = IMG_Load("stop.png");
        image_sur[2] = IMG_Load("clear.png");
        image_sur[3] = IMG_Load("up.png");
        image_sur[4] = IMG_Load("down.png");
        image_sur[5] = IMG_Load("upload.png");

        // initial value
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                table[i][j] = table_in[i][j];
                if(table[i][j]) 
                    button_table[i][j]->color = SELECTED_COLOR;
                else 
                    button_table[i][j]->color = CELL_COLOR;

            }
        }
        file_name = name;
        file(file_name, "w");
    }

    int full_num(int i, int j)
    {
        int num = 0;

        // Subtract the current cell's value if it is alive
        if (table[i][j]) {
            num = -1;
        }

        // Checking neighbors using loop and temporary variables
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                int row = i + x;
                int col = j + y;
                if (row >= 0 && row < N && col >= 0 && col < N) {
                    if (table[row][col]) {
                        num++;
                    }
                }
                if (num == 4) return num;
            }
        }

        return num;
    }

    void state()
    {   
        cnt++;   
        // check each square and put it in answer
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                int count_full_square = full_num(j, k);

                if (table[j][k])
                {
                    if (count_full_square == 2 || count_full_square == 3)
                        answer[j][k] = 1;
                    else
                        answer[j][k] = 0;
                }
                else
                {
                    if (count_full_square == 3)
                        answer[j][k] = 1;
                    else
                        answer[j][k] = 0;
                }

                // update color of button
                if(answer[j][k]) 
                    button_table[j][k]->color = SELECTED_COLOR;
                else 
                    button_table[j][k]->color = CELL_COLOR;
            }
        }

        // update file
        file(file_name, "w", cnt);

        // copy answer in table
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                table[j][k] = answer[j][k];
            }
        }
    }

    void display()
    {
        // cout << endl;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                cout << table[i][j];
                if (j != N - 1)
                    cout << " ";
            }
            cout << endl;
        }
    }

    void draw(){

        //rendering buttons
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++)
                button_table[i][j]->draw(rend);
        }
        
        //rendering lines
        SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
        for (int i = 0; i < N; i++){
            SDL_RenderDrawLine(rend, i * (WIDTH / N), 0, i * (WIDTH / N), HEIGHT);
            SDL_RenderDrawLine(rend, 0, i * (HEIGHT / N), WIDTH, i * (HEIGHT / N));
        }

        //rendering images
        // start image
        SDL_Texture* image_tex = SDL_CreateTextureFromSurface(rend, image_sur[0]);
        int imgW = 0;
        int imgH = 0;
        SDL_QueryTexture(image_tex, NULL, NULL, &imgW, &imgH);
        SDL_Rect img_rect = {WIDTH, HEIGHT / 2 - imgH / 2, imgW - 70, imgH - 70};
        SDL_RenderCopy(rend, image_tex, NULL, &img_rect);
        // stop image
        image_tex = SDL_CreateTextureFromSurface(rend, image_sur[1]);
        SDL_QueryTexture(image_tex, NULL, NULL, &imgW, &imgH);
        img_rect = {WIDTH, 0, imgW - 70, imgH - 70};
        SDL_RenderCopy(rend, image_tex, NULL, &img_rect);
        // clear image
        image_tex = SDL_CreateTextureFromSurface(rend, image_sur[2]);
        SDL_QueryTexture(image_tex, NULL, NULL, &imgW, &imgH);
        img_rect = {WIDTH, HEIGHT - 150, imgW - 100, imgH - 80};
        SDL_RenderCopy(rend, image_tex, NULL, &img_rect);
        // up image
        image_tex = SDL_CreateTextureFromSurface(rend, image_sur[3]);
        SDL_QueryTexture(image_tex, NULL, NULL, &imgW, &imgH);
        img_rect = {WIDTH, HEIGHT / 2 + 50, imgW - 210, imgH - 110};
        SDL_RenderCopy(rend, image_tex, NULL, &img_rect);
        // down image
        image_tex = SDL_CreateTextureFromSurface(rend, image_sur[4]);
        SDL_QueryTexture(image_tex, NULL, NULL, &imgW, &imgH);
        img_rect = {WIDTH, HEIGHT / 2 + 120, imgW - 220, imgH - 110};
        SDL_RenderCopy(rend, image_tex, NULL, &img_rect);
        // upload image
        image_tex = SDL_CreateTextureFromSurface(rend, image_sur[5]);
        SDL_QueryTexture(image_tex, NULL, NULL, &imgW, &imgH);
        img_rect = {WIDTH, HEIGHT * 3 / 4 - 50, imgW - 150, imgH - 150};
        SDL_RenderCopy(rend, image_tex, NULL, &img_rect);


        //rendering text
        SDL_Surface * surface = TTF_RenderText_Solid(font, to_string(cnt).c_str(), text_color);
        SDL_Texture * texture = SDL_CreateTextureFromSurface(rend, surface);
        int texW = 0;
        int texH = 0;
        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
        SDL_Rect dstrect = {WIDTH + PANEL_WIDTH / 2, HEIGHT / 4, texW, texH };
        SDL_RenderCopy(rend, texture, NULL, &dstrect);
    }

    void run(){
        bool running = true;
        int lock = 0;
        while(running){
            while(SDL_PollEvent(&event)){
                if (event.type == SDL_QUIT){
                    running = false;
                    SDL_Quit();
                    break;
                }
                if (event.type == SDL_MOUSEBUTTONDOWN && lock != 1){
                    lock = handle_click(event.motion.x, event.motion.y);
                }
                if (event.type == SDL_MOUSEBUTTONDOWN && lock == 1){
                    lock = handle_click(event.motion.x, event.motion.y);
                }
            }
            SDL_SetRenderDrawColor(rend, 200, 200, 255, 255);
            SDL_RenderClear(rend);
            if (lock == 1) 
            {
                state();
                std::this_thread::sleep_for(std::chrono::seconds{speed});
            }
            if (lock == 3) {
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < N; j++) {
                        table[i][j] = false;
                        button_table[i][j]->color = CELL_COLOR;
                    }
                }
                cnt = 0;
                ofstream file;
                file.open(file_name + ".txt");
                file.close();
            }
            draw();
            SDL_RenderPresent(rend);
        }
    }

    int handle_click(int x, int y){
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (button_table[i][j]->is_Clicked({x, y})) {
                    if(table[i][j]){
                        table[i][j] = false;
                        button_table[i][j]->color = CELL_COLOR;
                    }
                    else {
                        table[i][j] = true;
                        button_table[i][j]->color = SELECTED_COLOR;

                    }
                }
            }
        }
        if (start.is_Clicked({x, y})) return 1;
        if (stop.is_Clicked({x, y})) return 2;
        if (clear.is_Clicked({x, y})) return 3;
        if (up.is_Clicked({x, y})) 
        {
            speed++;
            return 1;
        }
        if (down.is_Clicked({x, y})) 
        {
            speed--;
            if (speed <= 1) speed = 1;
            return 1;
        }
        if (upload.is_Clicked({x, y})) {
            int k;
            cout << "Please Enter level: ";
            cin >> k;
            file(file_name, "r", k);
            return 2;
        };

        return 0;
    }

    void file(string name, string mode, int level = 1) {

        // add in each level to file
        if (mode == "w") {
            name =  name + ".txt";
            FILE * file = fopen(name.c_str(), "a");
            fprintf(file, "%d\n", cnt);
            for (int i = 0; i < N; i++) {
                for (int j = 0 ; j < N; j++) {
                    if (j == N - 1) 
                    {
                        fprintf(file, "%d\n", table[i][j]);
                        continue;
                    }
                    fprintf(file, "%d ", table[i][j]);
                }
            }
            fclose(file);
        }

        // upload to GOL
        if (mode == "r") {
            name = name + ".txt";
            ifstream input_file;
            input_file.open(name);
            for (int k = 0; k < cnt; k++) {
                int tmp_level;
                bool tmp_table[N][N];
                input_file >> tmp_level;
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < N; j++) {
                        input_file >> tmp_table[i][j];
                    }
                }
                if (tmp_level == level) {
                    for (int i = 0; i < N; i++) {
                        for (int j = 0; j < N; j++) {
                            table[i][j] == tmp_table[i][j];
                            if(table[i][j]) {
                                button_table[i][j]->color = SELECTED_COLOR;
                            }
                            else {
                                button_table[i][j]->color = CELL_COLOR;
                            }
                        }
                    }
                    break;
                }
            }
            input_file.close();
        }
    }
};



int main(int argc, char* argv[]){

    string name;
    cout << "Please Enter File Name: ";
    cin >> name;
    cout << N << " * " << N << endl;
    bool table[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> table[i][j];
        }
    }
    GOL game = GOL(table, name);
    game.run();

    return 0;
}