#include "ColorHarmonyInterface.h"

// CONSTRUCT
ColorHarmonyInterface::ColorHarmonyInterface()
{
    // Window
    this->set_position(Gtk::WIN_POS_CENTER);
    this->set_title("ColorHarmony");
    this->set_border_width(10);

    // FileChooser
    this->widget_fileChooser.set_label("Choose File");
    this->widget_colorApply.set_label("Apply");
    this->widget_saveFile.set_label("Save File");

    // ComboBox
    this->widget_modeSelect.append("Analogous");
    this->widget_modeSelect.append("Monochromatic");
    this->widget_modeSelect.append("Complementary");
    this->widget_modeSelect.append("Split Complementary");
    this->widget_modeSelect.append("Triadic");
    this->widget_modeSelect.append("Square");
    this->widget_modeSelect.append("Debug");
    this->widget_modeSelect.set_active(0);

    // Options Entry
    this->widget_degreeSelect.set_tooltip_text("Color Surface Degree");
    this->widget_degreeSelect.set_text("10");
    this->widget_degreeSelect.set_max_length(3);

    // Label Button
    this->widget_histo.set_label("Dominant Color Histogram");
    this->widget_kmean.set_label("Dominant Color K-Mean");

    // Grid Margins
    this->widget_grid.set_margin_left(20);
    this->widget_grid.set_margin_right(20);
    this->widget_grid.set_margin_top(20);
    this->widget_grid.set_margin_bottom(20);
    this->widget_grid.set_column_spacing(20);
    this->widget_grid.set_row_spacing(20);

    // Lock before upload
    this->widget_colorSelect.set_sensitive(false);
    this->widget_modeSelect.set_sensitive(false);
    this->widget_degreeSelect.set_sensitive(false);
    this->widget_colorApply.set_sensitive(false);
    this->widget_saveFile.set_sensitive(false);
    this->widget_histo.set_sensitive(false);
    this->widget_kmean.set_sensitive(false);

    // Grid Attach
    // Images
    this->widget_grid.attach(this->widget_imageInput, 0, 0, 5, 5);
    this->widget_grid.attach(this->widget_imageOutput, 5, 0, 5, 5);

    // Open
    widget_grid.attach(widget_fileChooser, 0, 5, 10, 1);

    /* FRAME COLOR */
    // Color K-mean
    this->widget_grid.attach(this->widget_kmean, 0, 6, 5, 1);
    // Color Histo
    this->widget_grid.attach(this->widget_histo, 0, 7, 5, 1);
    // Color selected
    this->widget_grid.attach(this->widget_colorSelect, 0, 8, 5, 1);

    /* FRAME SETTING */
    this->widget_grid.attach(this->widget_modeSelect, 5, 6, 5, 1);
    this->widget_grid.attach(this->widget_degreeSelect, 5, 7, 5, 1);
    this->widget_grid.attach(this->widget_colorApply, 5, 8, 5, 1);

    // Save
    this->widget_grid.attach(this->widget_saveFile, 0, 9, 10, 1);

    this->add(this->widget_grid);

    // Actions
    this->widget_fileChooser.signal_clicked().connect(sigc::mem_fun(*this, &ColorHarmonyInterface::fileChooserClicked));
    this->widget_colorApply.signal_clicked().connect(sigc::mem_fun(*this, &ColorHarmonyInterface::colorApplyClicked));
    this->widget_saveFile.signal_clicked().connect(sigc::mem_fun(*this, &ColorHarmonyInterface::saveFileClicked));
    this->widget_degreeSelect.signal_changed().connect(sigc::mem_fun(*this, &ColorHarmonyInterface::entryDegreeInsert));

    this->widget_histo.signal_clicked().connect(sigc::mem_fun(*this, &ColorHarmonyInterface::computeColorHisto));
    this->widget_kmean.signal_clicked().connect(sigc::mem_fun(*this, &ColorHarmonyInterface::computeColorKMean));
    // Show
    this->show_all();
}

// DESTRUCT
ColorHarmonyInterface::~ColorHarmonyInterface() {}

// FILE CHOOSER
void ColorHarmonyInterface::fileChooserClicked()
{
    // Dialog
    Gtk::FileChooserDialog dialog("Please choose a PPM file", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);
    dialog.set_transient_for(*this);

    // Location
    dialog.set_current_folder(dialog.get_current_folder());

    // Filters
    auto filter_ppm = Gtk::FileFilter::create();
    filter_ppm->set_name("PPM files");
    filter_ppm->add_pattern("*.ppm");
    dialog.add_filter(filter_ppm);
    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog.add_filter(filter_any);

    // Run
    const int result = dialog.run();

    // Handle response
    switch (result)
    {
    case Gtk::RESPONSE_OK:
    {
        // Load Images
        this->data_pathImgIn = dialog.get_filename();
        std::cout << ">>> File : " << this->data_pathImgIn << std::endl;

        // Clear
        this->widget_imageInput.clear();
        this->widget_imageOutput.clear();

        // Resize
        auto pixbuf = Gdk::Pixbuf::create_from_file(this->data_pathImgIn);
        auto scaled = pixbuf->scale_simple(512, 512, Gdk::INTERP_BILINEAR);
        this->widget_imageInput.set(scaled);
        this->widget_imageOutput.set(scaled);

        // Unlock options
        this->widget_colorSelect.set_sensitive(true);
        this->widget_modeSelect.set_sensitive(true);
        this->widget_degreeSelect.set_sensitive(true);
        this->widget_colorApply.set_sensitive(true);
        this->widget_saveFile.set_sensitive(true);
        this->widget_kmean.set_sensitive(true);
        this->widget_histo.set_sensitive(true);

        // Change default color
        Gdk::Color c;
        c.set_rgb_p(1.0f, 0.0f, 0.0f);
        this->widget_colorSelect.set_color(c);
    }
    case Gtk::RESPONSE_CANCEL:
    {
        printf(">>> Cancel clicked (FileChooser)\n");
        break;
    }
    default:
    {
        printf(">>> Unexpected button clicked (FileChooser)\n");
        break;
    }
    }
}

// APPLY COLOR
void ColorHarmonyInterface::colorApplyClicked()
{
    // Data Color
    this->data_color = widget_colorSelect.get_rgba();
    float r = this->data_color.get_red() * 255;
    float g = this->data_color.get_green() * 255;
    float b = this->data_color.get_blue() * 255;

    // Data Degree
    std::string sBandWidth = this->widget_degreeSelect.get_text();

    // Mode Selected
    int mode = this->widget_modeSelect.get_active_row_number();

    // Apply Changes
    initAndApplyImage(this->data_pathImgIn, mode, r, g, b, stof(sBandWidth));

    // Write Output Resized
    auto pixbuf = Gdk::Pixbuf::create_from_file((char *)"output.ppm");
    auto scaled = pixbuf->scale_simple(512, 512, Gdk::INTERP_BILINEAR);
    this->widget_imageOutput.set(scaled);

    // FileHasChanged
    this->hasChanged = true;
}

// SAVE FILE
void ColorHarmonyInterface::saveFileClicked()
{
    // Dialog
    Gtk::FileChooserDialog dialog("Save file", Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Save", Gtk::RESPONSE_ACCEPT);
    dialog.set_transient_for(*this);

    // Location
    dialog.set_current_folder(dialog.get_current_folder());

    // Confirm overwrite
    dialog.set_do_overwrite_confirmation(true);

    // Default Name
    dialog.set_current_name("output.ppm");

    // Run
    const int result = dialog.run();

    // Handle response
    switch (result)
    {
    case Gtk::RESPONSE_ACCEPT:
    {
        saveImage(dialog.get_filename(), this->hasChanged, this->data_pathImgIn);
        break;
    }
    case Gtk::RESPONSE_CANCEL:
    {
        printf(">>> Cancel clicked (FileChooser)\n");
        break;
    }
    default:
    {
        printf(">>> Unexpected button clicked (FileChooser)\n");
        break;
    }
    }
}

// DEGREE INPUT
void ColorHarmonyInterface::entryDegreeInsert()
{
    int length = this->widget_degreeSelect.get_text_length();
    std::string input = this->widget_degreeSelect.get_text();

    if (!(checkNumber(input)))
    {
        this->widget_degreeSelect.delete_text(length - 1, length);
    }
    if (length > 0)
    {
        if (std::stoi(input) > 360)
        {
            this->widget_degreeSelect.set_text("360");
        }
    }
}

// CHECK STRING
bool ColorHarmonyInterface::checkNumber(std::string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ')
        {
            return false;
        }
        else if (isdigit(str[i]) == false)
        {
            return false;
        }
    }
    return true;
}

// Most Present Color Histogram
void ColorHarmonyInterface::computeColorHisto()
{
    // Change Color Selected
    double hue = histoDominantColor(this->data_pathImgIn);
    Gdk::Color c;
    c.set_hsl(hue, 1.0f, 0.5f);
    this->widget_colorSelect.set_color(c);
}

void ColorHarmonyInterface::computeColorKMean()
{
    // Load Kmean Colors
    Vec3Color c_kmean = kMeansDominantColor(this->data_pathImgIn, 5, 5);
    Gdk::Color c;
    c.set_rgb_p((float)c_kmean.getR() / 255, (float)c_kmean.getG() / 255, (float)c_kmean.getB() / 255);
    this->widget_colorSelect.set_color(c);
}