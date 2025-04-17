#include "lib.h"
#include "config/config.h"

vector<string> buffer; // Change buffer to a single string instead of a vector<string>

void buildReverseAccentMap(
    const unordered_map<string, unordered_map<char, string>>& accentMap,
    unordered_map<string, pair<string, char>>& reverseMap
) {
    for (const auto& [baseChar, toneMap] : accentMap) {
        for (const auto& [toneChar, composedChar] : toneMap) {
            reverseMap[composedChar] = {baseChar, toneChar};
        }
    }
}

bool isEnglish = false;
bool modeSwitchPressed = false;

int main() {
    // loadConfig("config.ini");
    // if (!getConfigBool("enabled")) return 0;
    // // daemonize();

    Display *display = XOpenDisplay(nullptr); // X11
    if (!display) {
        cerr << "Cannot open display. Ensure X server is running.\n";
        return 1;
    }
    

    Window root = DefaultRootWindow(display);
    int opcode, event, error;

    if (!XQueryExtension(display, "XInputExtension", &opcode, &event, &error)) {
        cerr << "XInputExtension not available\n";
        XCloseDisplay(display);
        return 1;
    }

    XIEventMask evmask; // Event type
    unsigned char mask[(XI_LASTEVENT + 7) / 8] = {0}; // Create byte array ceil(XI_Lastevent / 8)
    XISetMask(mask, XI_KeyPress); // Listen only for key press events
    XISetMask(mask, XI_FocusIn);
    XISetMask(mask, XI_ButtonPress);

    evmask.deviceid = XIAllDevices; // Listen from all devices
    evmask.mask_len = sizeof(mask);
    evmask.mask = mask;
    XISelectEvents(display, root, &evmask, 1);

    cout << "Now running\n";
    // buildReverseAccentMap(accentMap, reverseAccentMap);
    // for (const auto& [composedChar, baseAndTone] : reverseAccentMap) {
    //     const string& base = baseAndTone.first;
    //     char tone = baseAndTone.second;
    //     cout << "Composed: " << composedChar
    //          << " => Base: " << base
    //          << ", Tone: " << tone << endl;
    // }
    
    int cnt = 0;
    int fixBackspace = 0;
    
    while (true) {
        XEvent ev; // Holds data for an event
        XNextEvent(display, &ev);
        if (ev.xcookie.type == GenericEvent && ev.xcookie.extension == opcode) { // Handle event
            XGetEventData(display, &ev.xcookie); // Get data
            if (ev.xcookie.evtype == XI_FocusIn) {
                buffer.clear();
                cout << "[FocusIn] Reset buffer\n";
            }
            if (ev.xcookie.evtype == XI_ButtonPress) {
                buffer.clear();
                cout << "[Button Press] Reset buffer\n";
            }
            if (ev.xcookie.evtype == XI_KeyPress) { // Check if key press
                XIDeviceEvent* xievent = (XIDeviceEvent*)ev.xcookie.data; // Get keycode

                if (handleModifiersAndControlKeys(xievent, display, buffer, isEnglish)) {
                    continue;
                }
                char c = keycode_to_char(display, xievent->detail);

                if (isascii(c) && isprint(c) && isEnglish == false) {
                    cout << "Lan " << ++cnt << endl; 
                    cout << "Ki tu: " << c << endl; 

                    cout << "Buffer before: ";
                    for (auto &&i : buffer)
                    {
                        cout << i;
                    }
                    cout << endl;
                    bool foundBs = false;

                    if ((int)buffer.size() >= 1) {
                        if ((telexMap.find(string(1, c)) != telexMap.end()) || (c == 'w')) { // Xu li khi khong co w
                            buffer = handleTelexTransform(buffer, display, c, foundBs);
                            if (foundBs)
                                fixBackspace = (int)buffer.size();
                        } 
                        else if (isXJSRF(c)) {
                            buffer = handleXJSRF(buffer, display, c, foundBs);
                            if (foundBs){
                                fixBackspace = (int)buffer.size();
                            }
                        }
                    }  
                    if (foundBs == false) {
                        // cout << "FOUND \n"; 
                        buffer.push_back(string(1, c)); // Append character to the buffer
                    }

                    cout << "FIX " << fixBackspace << endl;
                    if (fixBackspace) {
                        for (int i = 0; i < fixBackspace + 1; i++) {
                            buffer.push_back("~");
                        }
                        // system("xmodmap -e 'keycode 127 = Pause'");
                        
                    }
                    // sendEnd(display);
                    fixBackspace = 0;

                    cout << "Buffer after: ";
                    for (auto &&i : buffer)
                    {
                        cout << i;
                    }
                    cout << endl << endl;
                }
                else {
                    continue;
                }
                XFreeEventData(display, &ev.xcookie);
            }
        }
    }
    
    XCloseDisplay(display);
    return 0;
}