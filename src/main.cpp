// This file was developed by Thomas Müller <thomas94@gmx.net>.
// It is published under the BSD-style license contained in the LICENSE.txt file.

#include "../include/ImageViewer.h"

#include <args.hxx>

#include <iostream>

using namespace args;
using namespace std;

namespace {
    int mainFunc(int argc, char* argv[]) {
        ArgumentParser parser{
            "Inspection tool for images with a high dynamic range.",
            "This goes after the options.",
        };

        HelpFlag help(parser, "help",
            "Display this help menu",
            {'h', "help"}
        );

        ValueFlag<float> exposure{parser, "exposure",
            "Initial exposure setting of the viewer.",
            {'e', "exposure"},
        };

        PositionalList<string> imageFiles{parser, "images",
            "The image files to be opened by the viewer.",
        };

        // Parse command line arguments and react to parsing
        // errors using exceptions.
        try {
            parser.ParseCLI(argc, argv);
        } catch (args::Help) {
            std::cout << parser;
            return 0;
        } catch (args::ParseError e) {
            std::cerr << e.what() << std::endl;
            std::cerr << parser;
            return -1;
        } catch (args::ValidationError e) {
            std::cerr << e.what() << std::endl;
            std::cerr << parser;
            return -2;
        }

        // Init nanogui application
        nanogui::init();

        {
            auto app = make_unique<ImageViewer>();
            app->drawAll();
            app->setVisible(true);

            for (const auto imageFile : get(imageFiles)) {
                app->addImage(make_shared<Image>(imageFile));
            }

            app->fitAllImages();

            if (exposure) {
                app->setExposure(get(exposure));
            }

            nanogui::mainloop();
        }

        nanogui::shutdown();

        return 0;
    }
}

int main(int argc, char* argv[]) {
    try {
        mainFunc(argc, argv);
    } catch (const runtime_error& e) {
        cerr << "Uncaught exception: "s + e.what() << endl;
        return 1;
    }
}