#include <Engine.h>
#include <Editor.h>
int main(int argc, char *argv[])
{
    Soarscape::PublicSingleton<Soarscape::Engine>::getInstance().startEngine(argc, argv);
    Soarscape::PublicSingleton<Soarscape::Editor>::getInstance().initialize(argc, argv);
    Soarscape::PublicSingleton<Soarscape::Editor>::getInstance().run();
    Soarscape::PublicSingleton<Soarscape::Engine>::getInstance().shutdownEngine();
}
