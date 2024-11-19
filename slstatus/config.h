/* See LICENSE file for copyright and license details. */

/* interval between updates (in ms) */
const unsigned int interval = 1300;

/* text to show if no value can be retrieved */
static const char unknown_str[] = "n/a";

/* maximum output string length */
#define MAXLEN 2048

/*fetch brightness */
static const char *
brightness(const char *unused) {
    static char temp[16];        // Buffer for the brightness value
    char buffer[32];             // Temporary buffer for formatted output
    FILE *fp;                    // Declare the FILE pointer

    fp = popen("brightnessctl -m acpi_video0 | awk -F, '{print substr($4, 0, length($4)-1)}'", "r");
    if (fp == NULL) {
        strcpy(temp, "N/A");
    } else {
        // Read the brightness value into temp
        fgets(temp, sizeof(temp), fp);
        pclose(fp);
        
        // Strip newline characters from temp

        // Format the output string in buffer
        snprintf(buffer, sizeof(buffer), "\uf522 %s", temp); // Only use temp here

        // Copy formatted output back to temp
        strncpy(temp, buffer, sizeof(temp) - 1);
        temp[strcspn(temp, "\n")] = '\0'; // Remove newline if present
        temp[sizeof(temp) - 1] = '\0'; // Ensure null-termination
    }
   
    return temp;  // Return the brightness value stored in temp
}



/* fetch volume */
const char *
volume(const char *unused) {
    static char vol[16];  // Increased size to accommodate longer strings
    FILE *fp;

    // Check if muted
    fp = popen("pamixer --get-mute", "r");
    if (fp == NULL) {
        strcpy(vol, "N/A");
    } else {
        char mute_status[8];  // Buffer to hold mute status
        fgets(mute_status, sizeof(mute_status), fp);
        pclose(fp);

        // Check if the output is "true\n" or "false\n"
        if (strcmp(mute_status, "true\n") == 0) {
            strcpy(vol, "\ueee8 ");
            return vol;
        }
    }

    // Get volume percentage if not muted
    fp = popen("pamixer --get-volume", "r");
    if (fp == NULL) {
        strcpy(vol, "N/A");
    } else {
        int current_volume;
        if (fscanf(fp, "%d", &current_volume) == 1) {
            snprintf(vol, sizeof(vol), "\uf028 %d", current_volume); // Format output
        } else {
            strcpy(vol, "N/A");
        }
        pclose(fp);
    }

    return vol;  // Return the constructed string
}



/*
 * function            description                     argument (example)
 *
 * battery_perc        battery percentage              battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_remaining   battery remaining HH:MM         battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_state       battery charging state          battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * cat                 read arbitrary file             path
 * cpu_freq            cpu frequency in MHz            NULL
 * cpu_perc            cpu usage in percent            NULL
 * datetime            date and time                   format string (%F %T)
 * disk_free           free disk space in GB           mountpoint path (/)
 * disk_perc           disk usage in percent           mountpoint path (/)
 * disk_total          total disk space in GB          mountpoint path (/)
 * disk_used           used disk space in GB           mountpoint path (/)
 * entropy             available entropy               NULL
 * gid                 GID of current user             NULL
 * hostname            hostname                        NULL
 * ipv4                IPv4 address                    interface name (eth0)
 * ipv6                IPv6 address                    interface name (eth0)
 * kernel_release      `uname -r`                      NULL
 * keyboard_indicators caps/num lock indicators        format string (c?n?)
 *                                                     see keyboard_indicators.c
 * keymap              layout (variant) of current     NULL
 *                     keymap
 * load_avg            load average                    NULL
 * netspeed_rx         receive network speed           interface name (wlan0)
 * netspeed_tx         transfer network speed          interface name (wlan0)
 * num_files           number of files in a directory  path
 *                                                     (/home/foo/Inbox/cur)
 * ram_free            free memory in GB               NULL
 * ram_perc            memory usage in percent         NULL
 * ram_total           total memory size in GB         NULL
 * ram_used            used memory in GB               NULL
 * run_command         custom shell command            command (echo foo)
 * swap_free           free swap in GB                 NULL
 * swap_perc           swap usage in percent           NULL
 * swap_total          total swap size in GB           NULL
 * swap_used           used swap in GB                 NULL
 * temp                temperature in degree celsius   sensor file
 *                                                     (/sys/class/thermal/...)
 *                                                     NULL on OpenBSD
 *                                                     thermal zone on FreeBSD
 *                                                     (tz0, tz1, etc.)
 * uid                 UID of current user             NULL
 * uptime              system uptime                   NULL
 * username            username of current user        NULL
 * vol_perc            OSS/ALSA volume in percent      mixer file (/dev/mixer)
 *                                                     NULL on OpenBSD/FreeBSD
 * wifi_essid          WiFi ESSID                      interface name (wlan0)
 * wifi_perc           WiFi signal in percent          interface name (wlan0)
 */

static const struct arg args[] = {
    /* function           format                 argument */
    { datetime,           " \uef37 %s",          "%m-%d~%H:%M" },
    { ram_used,           " | \uefc5 %s",        NULL },
    { battery_remaining,  " | %s",               "BAT0"},              
    { battery_state,      "%s",                 "BAT0"},
    { battery_perc,       "%s%%",                "BAT0" },
    { volume,             " | %s",               NULL },
    { brightness,         " | %s",               "acpi_video0" },
    { wifi_perc,          " | \uf1eb %s%% ",     "wlan0" }, 
};

