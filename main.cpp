#include <gtkmm/main.h>
#include "ColorHarmonyInterface.h"

int main(int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);
  ColorHarmonyInterface colorHarmonyGTK;
  Gtk::Main::run(colorHarmonyGTK);

  return 0;
}