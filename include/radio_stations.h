#ifndef RADIO_STATIONS_H
#define RADIO_STATIONS_H

#include <Arduino.h>

struct InternetRadioStation {
    const char* name;
    const char* genre;
    const char* url;
    const char* country;
};

// Popular Internet Radio Stations
// Add or modify as needed
const InternetRadioStation INTERNET_RADIO_STATIONS[] = {
    // Rock
    {"Classic Rock Florida", "Classic Rock", "http://airspectrum.cdnstream1.com:8018/1648_192", "USA"},
    {"181.FM - Rock 181", "Rock", "http://listen.181fm.com/181-rock_128k.mp3", "USA"},
    {"Rock Radio", "Rock", "http://streamingV2.shoutcast.com/RockRadio", "UK"},
    
    // Pop
    {"181.FM - Star 90s", "90s Pop", "http://listen.181fm.com/181-star90s_128k.mp3", "USA"},
    {"Hit Radio FFH", "Pop", "http://mp3.ffh.de/radioffh/hqlivestream.mp3", "Germany"},
    {"Radio 538", "Pop", "http://18973.live.streamtheworld.com/RADIO538.mp3", "Netherlands"},
    
    // Electronic/Dance
    {"Digitally Imported - Trance", "Trance", "http://prem2.di.fm:80/trance", "USA"},
    {"Radio Record", "Dance", "http://radiorecord.hostingradio.ru/rr_main96.aacp", "Russia"},
    {"DI.FM - Chillout", "Chillout", "http://prem2.di.fm:80/chillout", "USA"},
    
    // Hip Hop / R&B
    {"181.FM - The Beat", "Hip Hop", "http://listen.181fm.com/181-beat_128k.mp3", "USA"},
    {"The Heat", "Hip Hop/R&B", "http://stream.the-heat.co.uk:8000/", "UK"},
    
    // Jazz
    {"181.FM - Smooth Jazz", "Smooth Jazz", "http://listen.181fm.com/181-smoothjazz_128k.mp3", "USA"},
    {"Jazz Radio - Blues", "Jazz Blues", "http://jazzradio.ice.infomaniak.ch/jazzradio-blues.mp3", "France"},
    {"WBGO Jazz 88.3 FM", "Jazz", "http://wbgo.streamguys.net/wbgo128", "USA"},
    
    // Country
    {"181.FM - Highway 181", "Country", "http://listen.181fm.com/181-highway_128k.mp3", "USA"},
    {"181.FM - Classic Country", "Country", "http://listen.181fm.com/181-classic_country_128k.mp3", "USA"},
    
    // Classical
    {"Classical KUSC", "Classical", "http://radio.kusc.org:8000/kusc-m3u-128k", "USA"},
    {"Venice Classic Radio", "Classical", "http://174.36.206.197:8000/stream", "Italy"},
    
    // News/Talk
    {"NPR News", "News", "http://npr-ice.streamguys1.com/live.mp3", "USA"},
    {"BBC World Service", "News", "http://stream.live.vc.bbcmedia.co.uk/bbc_world_service", "UK"},
    
    // Alternative
    {"181.FM - The Buzz", "Alternative", "http://listen.181fm.com/181-buzz_128k.mp3", "USA"},
    {"KEXP 90.3 FM", "Alternative", "http://live-mp3-128.kexp.org/kexp128.mp3", "USA"},
    
    // Oldies
    {"181.FM - Good Time Oldies", "Oldies", "http://listen.181fm.com/181-goodtime_128k.mp3", "USA"},
    {"Oldies 104", "50s/60s", "http://oldies104.stream.laut.fm/oldies104", "USA"},
    
    // Metal
    {"181.FM - Heavy Metal", "Metal", "http://listen.181fm.com/181-heavymetal_128k.mp3", "USA"},
    {"Metal Rock Radio", "Metal", "http://kathy.torontocast.com:2670/stream", "USA"},
    
    // Reggae
    {"181.FM - Reggae Roots", "Reggae", "http://listen.181fm.com/181-reggae_128k.mp3", "USA"},
    
    // Latin
    {"181.FM - Salsa", "Salsa", "http://listen.181fm.com/181-salsa_128k.mp3", "USA"},
    {"Latina FM", "Latin Pop", "http://streamplus30.leonex.de:22460/;", "Spain"},
    
    // Christian
    {"181.FM - Praise", "Christian", "http://listen.181fm.com/181-praise_128k.mp3", "USA"},
    
    // Comedy
    {"181.FM - Comedy Club", "Comedy", "http://listen.181fm.com/181-comedy_128k.mp3", "USA"}
};

const int INTERNET_RADIO_STATION_COUNT = sizeof(INTERNET_RADIO_STATIONS) / sizeof(InternetRadioStation);

// Helper function to get station by index
inline const InternetRadioStation* getInternetRadioStation(int index) {
    if (index >= 0 && index < INTERNET_RADIO_STATION_COUNT) {
        return &INTERNET_RADIO_STATIONS[index];
    }
    return nullptr;
}

// Helper function to search stations by genre
inline int countStationsByGenre(const char* genre) {
    int count = 0;
    for (int i = 0; i < INTERNET_RADIO_STATION_COUNT; i++) {
        if (strcmp(INTERNET_RADIO_STATIONS[i].genre, genre) == 0) {
            count++;
        }
    }
    return count;
}

// Get unique genres
inline void getUniqueGenres(String* genres, int& count) {
    count = 0;
    for (int i = 0; i < INTERNET_RADIO_STATION_COUNT; i++) {
        String genre = INTERNET_RADIO_STATIONS[i].genre;
        bool found = false;
        
        for (int j = 0; j < count; j++) {
            if (genres[j] == genre) {
                found = true;
                break;
            }
        }
        
        if (!found && count < 20) {
            genres[count++] = genre;
        }
    }
}

#endif // RADIO_STATIONS_H
