#include "bsl/Log.hpp"

#define OLC_PGE_APPLICATION
#include "third_party/olcPixelGameEngine.h"
#include "Msick.hpp"


//ffmpeg is the GOAT
//Get-ChildItem *.m4a | ForEach-Object { ffmpeg -i $_.FullName -b:a 192k "$($_.BaseName).mp3" }

//good eough for now, need to refubish later

int main() {
    bb::log::init("log.txt");

    try {
        Msick msick{};

        if (msick.Construct(256, 144, 2, 2, false, true, false, false))
            msick.Start();
    }
    catch (std::exception& e) {
        bb::log::error(e);
    }

    return 0;
}