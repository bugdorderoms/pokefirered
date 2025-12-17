# Map JSON data

MAPS_DIR = $(DATA_ASM_SUBDIR)/maps
LAYOUTS_DIR = $(DATA_ASM_SUBDIR)/layouts
TILESETS_DIR = $(DATA_ASM_SUBDIR)/tilesets

MAP_DIRS := $(dir $(wildcard $(MAPS_DIR)/*/map.json))
MAP_CONNECTIONS := $(patsubst $(MAPS_DIR)/%/,$(MAPS_DIR)/%/connections.inc,$(MAP_DIRS))
MAP_EVENTS := $(patsubst $(MAPS_DIR)/%/,$(MAPS_DIR)/%/events.inc,$(MAP_DIRS))
MAP_HEADERS := $(patsubst $(MAPS_DIR)/%/,$(MAPS_DIR)/%/header.inc,$(MAP_DIRS))
MAP_JSONS := $(patsubst $(MAPS_DIR)/%/,$(MAPS_DIR)/%/map.json,$(MAP_DIRS))

# Generate constants for map events, which depend on data that's distributed across the map.json files.
# There's a lot of map.json files, so we print an abbreviated output with echo.
include/constants/map_event_ids.h: $(MAP_JSONS)
	@$(MAPJSON) event_constants firered $^ include/constants/map_event_ids.h
	@echo "$(MAPJSON) event_constants firered <MAP_JSONS> include/constants/map_event_ids.h"

$(TILESETS_DIR)/tilesets_table.inc include/constants/tilesets.h: $(TILESETS_DIR)/tilesets.json
	$(MAPJSON) tilesets firered $<

$(MAPS_DIR)/%/header.inc: $(MAPS_DIR)/%/map.json
	$(MAPJSON) map firered $< $(LAYOUTS_DIR)/layouts.json
$(MAPS_DIR)/%/events.inc: $(MAPS_DIR)/%/header.inc ;
$(MAPS_DIR)/%/connections.inc: $(MAPS_DIR)/%/events.inc ;

$(MAPS_DIR)/groups.inc: $(MAPS_DIR)/map_groups.json
	$(MAPJSON) groups firered $<
$(MAPS_DIR)/connections.inc: $(MAPS_DIR)/groups.inc ;
$(MAPS_DIR)/events.inc: $(MAPS_DIR)/connections.inc ;
$(MAPS_DIR)/headers.inc: $(MAPS_DIR)/events.inc ;
include/constants/map_groups.h: $(MAPS_DIR)/headers.inc ;

$(LAYOUTS_DIR)/layouts.inc: $(LAYOUTS_DIR)/layouts.json
	$(MAPJSON) layouts firered $<
$(LAYOUTS_DIR)/layouts_table.inc: $(LAYOUTS_DIR)/layouts.inc ;
include/constants/layouts.h: $(LAYOUTS_DIR)/layouts_table.inc ;

$(DATA_ASM_BUILDDIR)/maps.o: $(DATA_ASM_SUBDIR)/maps.s $(TILESETS_DIR)/tilesets_table.inc $(LAYOUTS_DIR)/layouts.inc $(LAYOUTS_DIR)/layouts_table.inc $(MAPS_DIR)/headers.inc $(MAPS_DIR)/groups.inc $(MAPS_DIR)/connections.inc $(MAP_CONNECTIONS) $(MAP_HEADERS)
	$(PREPROC) $< charmap.txt | $(CPP) -I include -nostdinc -undef -Wno-unicode - | $(AS) $(ASFLAGS) -o $@
$(DATA_ASM_BUILDDIR)/map_events.o: $(DATA_ASM_SUBDIR)/map_events.s $(MAPS_DIR)/events.inc $(MAP_EVENTS)
	$(PREPROC) $< charmap.txt | $(CPP) -I include -nostdinc -undef -Wno-unicode - | $(AS) $(ASFLAGS) -o $@

