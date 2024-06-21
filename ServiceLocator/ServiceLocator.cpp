#include <iostream>

#include "Audio.hpp"
#include "ConsoleAudio.h"
#include "Locator.h"
#include "LoggedAudio.h"

void EnableAudioLogging()
{
	Audio *service = new LoggedAudio(Locator::GetAudio());
	Locator::Provide(service);
}

int main()
{
    std::cout << "Hello World!\n";
    Locator::Initialize();
    const auto consoleAudio = new ConsoleAudio();
    Locator::Provide(consoleAudio);
    EnableAudioLogging();

    Audio& audio = Locator::GetAudio();
    audio.PlaySound(0);

    //Locator::GetAudio().PlaySound(0);
    //audio->PlaySound(0);
}
