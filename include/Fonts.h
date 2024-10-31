#pragma once

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
// Additional colors
#define BRIGHT_BLACK "\033[1;30m"      // Bright Black (Gray)
#define LIGHT_RED "\033[38;5;196m"     // Light Red
#define LIGHT_GREEN "\033[38;5;82m"    // Light Green
#define LIGHT_YELLOW "\033[38;5;226m"  // Light Yellow
#define LIGHT_BLUE "\033[38;5;93m"     // Light Blue
#define LIGHT_MAGENTA "\033[38;5;201m" // Light Magenta
#define LIGHT_CYAN "\033[38;5;51m"     // Light Cyan
#define LIGHT_WHITE "\033[38;5;255m"   // Light White (Bright White)

// Background colors
#define BACKGROUND_LIGHT_RED "\033[48;5;196m"     // Background Light Red
#define BACKGROUND_LIGHT_GREEN "\033[48;5;82m"    // Background Light Green
#define BACKGROUND_LIGHT_YELLOW "\033[48;5;226m"  // Background Light Yellow
#define BACKGROUND_LIGHT_BLUE "\033[48;5;93m"     // Background Light Blue
#define BACKGROUND_LIGHT_MAGENTA "\033[48;5;201m" // Background Light Magenta
#define BACKGROUND_LIGHT_CYAN "\033[48;5;51m"     // Background Light Cyan
#define BACKGROUND_LIGHT_WHITE "\033[48;5;255m"   // Background Light White

// fonts
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"
#define ITALIC "\033[3m"        // Note: Italics may not be supported in all terminals
#define STRIKETHROUGH "\033[9m" // Not widely supported
#define INVERT "\033[7m"        // Inverse colors