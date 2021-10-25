// SDL_Test.h: Includedatei für Include-Standardsystemdateien
// oder projektspezifische Includedateien.

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <vector>

// Defintions
constexpr double frame_rate = 60.0; // refresh rate
constexpr double frame_time = 1. / frame_rate;
constexpr unsigned frame_width = 1200; // Width of window in pixel
constexpr unsigned frame_height = 900; // Height of window in pixel
// Minimal distance of animals to the border of the screen
constexpr unsigned frame_boundary = 0;

// Helper function to initialize SDL
void init();


class animal {
private:
    SDL_Surface* window_surface_ptr_; // ptr to the surface on which we want the animal to be drawn, also non-owning
    SDL_Surface* image_ptr_; // The texture of the sheep (the loaded image), use load_surface_for

    // todo: Attribute(s) to define its position
protected:
    double pos_x_, pos_y_, v_x_, v_y_;

public:
    animal(const std::string& file_path, SDL_Surface* window_surface_ptr, double v_x, double v_y); // todo: The constructor has to load the
                                   // sdl_surface that corresponds to the texture

    ~animal(); // todo: Use the destructor to release memory and "clean up behind you"

    void draw(); // todo: Draw the animal on the screen <-> window_surface_ptr.
                   // Note that this function is not virtual, it does not depend
                   // on the static type of the instance

    virtual void move() = 0; // todo: Animals move around, but in a different
                               // fashion depending on which type of animal
    SDL_Surface* getwindow();
    SDL_Surface* getimage();
};

// Insert here:
// class sheep, derived from animal
class sheep : public animal {
    // todo

    // Ctor
public:
    sheep(SDL_Surface* sheep_surf);
    // Dtor
    ~sheep();
    // implement functions that are purely virtual in base class
    void move() override;
};


class wolf : public animal {
    // todo

    // Ctor
public:
    wolf(SDL_Surface* sheep_surf);
    // Dtor
    ~wolf();
    // implement functions that are purely virtual in base class
    void move() override;
};

// The "ground" on which all the animals live (like the std::vector
// in the zoo example).
class ground {
private:
    // Attention, NON-OWNING ptr, again to the screen
    SDL_Surface* window_surface_ptr_;

    // Some attribute to store all the wolves and sheep
    // here
    std::vector<animal*> zoo_;

public:
    ground(SDL_Surface* window_surface_ptr); // todo: Ctor
    ~ground();  // todo: Dtor, again for clean up (if necessary)
    void add_animal(animal* ani); // todo: Add an animal
    void update(); // todo: "refresh the screen": Move animals and draw them
    // Possibly other methods, depends on your implementation
    void setScreen_ptr(SDL_Surface* window_surface_ptr);
};

// The application class, which is in charge of generating the window
class application {

private:
    // The following are OWNING ptrs
    SDL_Window* window_ptr_;
    SDL_Surface* window_surface_ptr_;
    //SDL_Event window_event_;

    // Other attributes here, for example an instance of ground
    ground ground_;
    uint32_t tempsPrecedent, tempsActuel;

public:
    application(unsigned n_sheep, unsigned n_wolf); // Ctor
    ~application();                                 // dtor

    int loop(unsigned period); // main loop of the application.
                               // this ensures that the screen is updated
                               // at the correct rate.
                               // See SDL_GetTicks() and SDL_Delay() to enforce a
                               // duration the application should terminate after
                               // 'period' seconds
};