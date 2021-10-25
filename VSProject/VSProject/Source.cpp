// SDL_Test.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "project.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <random>
#include <string>

void init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("init():" + std::string(SDL_GetError()));

    // Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
        throw std::runtime_error("init(): SDL_image could not initialize! "
            "SDL_image Error: " +
            std::string(IMG_GetError()));
}

namespace {
    // Defining a namespace without a name -> Anonymous workspace
    // Its purpose is to indicate to the compiler that everything
    // inside of it is UNIQUELY used within this source file.

    SDL_Surface* load_surface_for(const std::string& path, SDL_Surface* window_surface_ptr) {

        if (!window_surface_ptr) {
            std::cout << "window not valid." << std::endl;
            return (nullptr);
        }
        SDL_Surface* surf = SDL_ConvertSurface(IMG_Load(path.c_str()), window_surface_ptr->format, 0);
        if (!surf) {
            std::cout << "Error during creation of surface." << std::endl;
            return (nullptr);
        }

        return (surf);
        // Helper function to load a png for a specific surface
        // See SDL_ConvertSurface

    }
} // namespace

/* Fonction de animal */

animal::animal(const std::string& file_path, SDL_Surface* window_surface_ptr, double v_x, double v_y)
    : window_surface_ptr_(window_surface_ptr), v_x_(v_x), v_y_(v_y) {
    this->pos_x_ = std::rand() % (frame_width - 2 * frame_boundary) + frame_boundary;
    this->pos_y_ = std::rand() % (frame_height - 2 * frame_boundary) + frame_boundary;

    this->image_ptr_ = load_surface_for(file_path, window_surface_ptr);
    //gère le alpha de l'image
    auto color_key = SDL_MapRGB(this->image_ptr_->format, 0, 0, 0);
    SDL_SetColorKey(this->image_ptr_, SDL_TRUE, color_key);
};

animal::~animal() {
    SDL_FreeSurface(this->image_ptr_);
    delete (this);
};

void animal::draw() {
    SDL_Rect area;
    area.x = this->pos_x_;
    area.y = this->pos_y_;
    area.w = this->image_ptr_->w;
    area.h = this->image_ptr_->h;
    SDL_BlitScaled(this->image_ptr_, NULL, this->window_surface_ptr_, &area);
}

SDL_Surface* animal::getwindow() {
    return this->window_surface_ptr_;
}

SDL_Surface* animal::getimage() {
    return this->image_ptr_;
}

/* Fonctions de sheep */
sheep::sheep(SDL_Surface* sheep_surf)
    : animal(std::string("./media/sheep.png"), sheep_surf, 10, 5) {}

sheep::~sheep() {}

void sheep::move() {
    int w_ = this->getwindow()->w - frame_boundary - this->getimage()->w;
    int h_ = this->getwindow()->h - frame_boundary - this->getimage()->h;

    this->pos_x_ += (frame_time * this->v_x_);
    this->pos_y_ += (frame_time * this->v_y_);

    if (this->pos_x_ < frame_boundary) {
        this->pos_x_ = frame_boundary;
        this->v_x_ = std::abs(this->v_x_);
    }
    if (this->pos_y_ < frame_boundary) {
        this->pos_y_ = frame_boundary;
        this->v_y_ = std::abs(this->v_y_);
    }
    if (this->pos_x_ < w_) {
        this->pos_x_ = w_;
        this->v_x_ = -std::abs(this->v_x_);
    }
    if (pos_y_ < h_) {
        this->pos_y_ = h_;
        this->v_y_ = -std::abs(this->v_y_);
    }
};


/* Fonction de woof */

wolf::wolf(SDL_Surface* wolf_surf)
    : animal(std::string("./media/wolf.png"), wolf_surf, 10, -10) {}

wolf::~wolf() {}

void wolf::move() {
    int w_ = this->getwindow()->w - frame_boundary - this->getimage()->w;
    int h_ = this->getwindow()->h - frame_boundary - this->getimage()->h;

    this->pos_x_ += (frame_time * this->v_x_);
    this->pos_y_ += (frame_time * this->v_y_);

    if (pos_x_ < frame_boundary) {
        this->pos_x_ = frame_boundary;
        this->v_x_ = std::abs(this->v_x_);
    }
    if (pos_y_ < frame_boundary) {
        this->pos_y_ = frame_boundary;
        this->v_y_ = std::abs(this->v_y_);
    }
    if (pos_x_ < w_) {
        this->pos_x_ = w_;
        this->v_x_ = -std::abs(this->v_x_);
    }
    if (pos_y_ < h_) {
        this->pos_y_ = h_;
        this->v_y_ = -std::abs(this->v_y_);
    }
}


/* Fonctions de ground */


ground::ground(SDL_Surface* window_surface_ptr)
    : window_surface_ptr_(window_surface_ptr), zoo_(std::vector<animal*>()) {}

ground::~ground() {
    for (auto animal_ : this->zoo_) {
        animal_->~animal();
    }
}

void ground::add_animal(animal* animal_) { this->zoo_.push_back(animal_); }

void ground::update() {
    SDL_FillRect(this->window_surface_ptr_, NULL, SDL_MapRGB(this->window_surface_ptr_->format, 0, 127, 0));
    for (animal* animal_ : this->zoo_) {
        animal_->move();
        animal_->draw();
    }

}

void ground::setScreen_ptr(SDL_Surface* window_surface_ptr) {
    this->window_surface_ptr_ = window_surface_ptr;
}




/* Fonctions application */

application::application(unsigned n_sheep, unsigned n_wolf)
    : tempsActuel(0), tempsPrecedent(0), ground_(ground(NULL)) {

    //window
    this->window_ptr_ = SDL_CreateWindow("Zoo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, frame_width, frame_height, 0);
    if (!this->window_ptr_) {
        // si la window n'est pas créer correctement
        std::cout << ("Could not create window: %s", SDL_GetError()) << std::endl;
        SDL_Delay(10000);
    }
    this->window_surface_ptr_ = SDL_GetWindowSurface(this->window_ptr_);
    SDL_FillRect(this->window_surface_ptr_, NULL, SDL_MapRGB(this->window_surface_ptr_->format, 0, 127, 0));

    this->ground_.setScreen_ptr(this->window_surface_ptr_);

    //animal
    std::srand((unsigned)time(nullptr));// initialise l'aléatoire des position x et y de l'animal;
    for (int i = 0; i < n_sheep; i++) {
        sheep* sheep_ = new sheep(this->window_surface_ptr_);
        sheep_->draw();
        this->ground_.add_animal(sheep_);
    }
    for (int i = 0; i < n_wolf; i++) {
        wolf* wolf_ = new wolf(this->window_surface_ptr_);
        wolf_->draw();
        this->ground_.add_animal(wolf_);
    }
    //todo window_event_


    SDL_UpdateWindowSurface(this->window_ptr_);
}

application::~application() {
    SDL_FreeSurface(this->window_surface_ptr_);
    SDL_DestroyWindow(this->window_ptr_);
    this->ground_.~ground();
    SDL_Quit();

    // window_envent ?
}

int application::loop(unsigned period) {
    while (this->tempsActuel < period) {
        this->tempsActuel = SDL_GetTicks();
        if (this->tempsActuel - this->tempsPrecedent >= frame_time) {
            this->ground_.update();
            SDL_UpdateWindowSurface(this->window_ptr_);
        }
        else /* Si ça fait moins du temps d'affichage d'une image depuis le dernier tour de boucle, on endort le programme le temps qu'il faut */
        {
            SDL_Delay(frame_time - (this->tempsActuel - this->tempsPrecedent));
        }

        this->tempsPrecedent = this->tempsActuel;
        /*
        * update est appelé à chaque image, se qui donne une grand vitesse au animaux
        * alors j'ai introdui un coefficient de ralentisement qui augmente le temps entre
        * chaque update.
        */
    }


    return 0;
}


