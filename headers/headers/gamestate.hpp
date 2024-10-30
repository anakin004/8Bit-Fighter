#pragma once

class Game {

    public:

        Game();
        ~Game();

        void start();

        void free();

    private:

        bool mRunning;


};