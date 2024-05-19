/*
 * layouts.cpp
 *
 *  Created on: Oct 11, 2023
 *      Author: Matthew Hinton
 */

#include "layouts.h"

void find_layoutfiles(fs::path layoutDir, int* layoutCountAddr, fs::path* layoutfiles) {
    *layoutCountAddr = 0;
    string filename;
    int end;
    for (fs::directory_entry const& entry : fs::recursive_directory_iterator(layoutDir)) {
        if (entry.is_regular_file()) {
            filename = entry.path().filename();
            if ((end = filename.length() - 1) >= 3) {
                if (filename.substr(end - 3, end) == ".lyt") {
                    if (layoutfiles != nullptr) layoutfiles[*layoutCountAddr] = entry.path();
                    (*layoutCountAddr)++;
                } else if ((end = filename.length() - 1) >= 6) {
                    if (filename.substr(end - 6, end) == ".layout") {
                        if (layoutfiles != nullptr) layoutfiles[*layoutCountAddr] = entry.path();
                        (*layoutCountAddr)++;
                    }
                }
            }
        }
    }
}

void layoutcounting (int* layoutCountAddr) {
    *layoutCountAddr = 4;
}

// note: implicitly assumes spacebar
const MetaLayout** loadMetaLayouts(fs::path layoutDir) {
    int layoutCount;
    fs::path* layoutfiles { nullptr };

    try {
        find_layoutfiles(layoutDir, &layoutCount, layoutfiles);
        layoutfiles = new fs::path [layoutCount];
        find_layoutfiles(layoutDir, &layoutCount, layoutfiles);
    } catch(const fs::filesystem_error& e) {
        std::cout << e.what() << '\n';
    }

    // print out list of files found by find_layoutfiles()
    cout << "Found layout files:" << endl
        << string(19, '-') << endl;
    for (int i{ }; i < layoutCount; i++) {
        cout << layoutfiles[i] << endl;
    }
    cout << endl;

    const MetaLayout** layoutArray = new const MetaLayout* [layoutCount];

    for (int i{ }; i < layoutCount; i++) {
        string layoutName, layoutCreator;
        char layoutKey[LayoutSize];

        char* layoutblock;

        const fs::path layoutfilename = layoutfiles[i];

        ifstream layoutfile (layoutfilename, ios::in | ios::ate);
        if (layoutfile.is_open()) {

            streampos layout_block_end = layoutfile.tellg();

            layoutfile.seekg(0, ios::beg);

            getline(layoutfile, layoutName);
            getline(layoutfile, layoutCreator);

            streampos beg_of_layout = layoutfile.tellg();

            int layout_block_size = layout_block_end - beg_of_layout;

            if (layout_block_size < (LayoutSize - 1) * 2) cout << "Incomplete layout in layoutfile " << layoutfiles[i] << "\n";
            else if (layout_block_size > (LayoutSize - 1) * 2) cout << "Improper layout in layoutfile " << layoutfiles[i] << "\n";
            else {
                layoutblock = new char [layout_block_size];
                layoutfile.seekg(beg_of_layout, ios::beg);
                layoutfile.read(layoutblock, layout_block_size);

                bool proper{ true };
                for (int j{1}; j < layout_block_size; j += 2) {
                    if (layoutblock[j] != ' ' && layoutblock[j] != '\n') proper = false;
                }

                if (proper) {
                    int index;
                    int k{ };
                    while (k < LayoutSize - 1) {
                        index = alpha_index[(int)layoutblock[k * 2]];
                        if (index < 0 || index >= LayoutSize) {
                            proper = false;
                            break;
                        }
                        layoutKey[index] = k + 1;
                        k++;
                    }
                    layoutKey[(int)alpha_index[(int)' ']] = 0;
                }
                if (!proper) cout << "Improper layout in layoutfile " << layoutfiles[i] << "\n";
            }

            layoutfile.close();

            layoutArray[i] = new const MetaLayout (layoutName, layoutCreator, layout_t(layoutKey));

        } else cout << "Could not open layoutfile \"" << layoutfiles[i] << "\"\n";
    }
    if (layoutfiles != nullptr) delete[] layoutfiles;

    // print out names of successfully loaded layouts
    cout << "Loaded layouts:" << endl
        << string(15, '-') << endl;
    for (int i{ }; i < layoutCount; i++) {
        cout << (*layoutArray[i]).name << endl;
    }
    cout << endl;

    return layoutArray;
}

void print_layout(const MetaLayout* lyt) {
    if (LayoutSize != 31) { cout << "Could not print layout \"" << (*lyt).name << "\"; LayoutSize != 31\n"; return; }
    cout << "Name: " << (*lyt).name << endl;
    cout << "Creator: " << (*lyt).creator << endl;
    for (int i{ }; i < 3; i++) {
        for (int j{ }; j < 10; j++) {
            cout << alphabet[(int)(*lyt).letter(10 * i + j + 1)] << ' '; }
        cout << endl;
    }
    cout << endl;
}

/*

void gen_alpha_index() {
    int extra_char_count = sizeof(extCharsList) / sizeof(char);
    for (int i{ }; i < extra_char_count; i++) {
        alpha_index[(int)extCharsList[i]] = -(i + 1);
    }
    for (unsigned i{ }; i < sizeof(alphabet) / sizeof(char); i++) {
        alpha_index[(int)alphabet[i]] = i;
    }
}


int main() {
    gen_alpha_index();

    fs::path layoutDir = fs::current_path();

    MetaLayout** layouts = loadMetaLayouts(layoutDir);

    printf("Number of layouts: %d\n\n", MetaLayout::numberof);

    // print all layouts
    cout << "Here is a list of all the loaded layouts:" << endl
        << string(41, '-') << endl;
    for (int i{ }; i < MetaLayout::numberof; i++)
        print_layout(layouts[i]);

    return 0;
}
*/
