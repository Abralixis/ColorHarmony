#ifndef COLOR_HARMONY_INTERFACE_H
#define COLOR_HARMONY_INTERFACE_H

#include <string>
#include <iostream>
#include <vector>

#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/button.h>
#include <gtkmm/colorbutton.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/filefilter.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/entry.h>
#include <gdkmm/rgba.h>
#include <gdkmm/pixbuf.h>

#include "tools.h"

class ColorHarmonyInterface : public Gtk::Window
{
private:
    // Widgets
    Gtk::Grid widget_grid;

    Gtk::Image widget_imageInput;
    Gtk::Image widget_imageOutput;

    Gtk::ColorButton widget_colorSelect;
    Gtk::ComboBoxText widget_modeSelect;
    Gtk::Entry widget_degreeSelect;

    Gtk::Button widget_fileChooser;
    Gtk::Button widget_colorApply;
    Gtk::Button widget_saveFile;

    Gtk::Button widget_histo;
    Gtk::Button widget_kmean;

    Gdk::RGBA data_color;

    std::string data_pathImgIn;

    // Flag
    bool hasChanged = false;

    // Signals
    virtual void fileChooserClicked();
    virtual void colorApplyClicked();
    virtual void saveFileClicked();
    virtual void entryDegreeInsert();

    virtual void computeColorHisto();
    virtual void computeColorKMean();

public:
    ColorHarmonyInterface();
    virtual ~ColorHarmonyInterface();

    virtual bool checkNumber(std::string str);
};

#endif