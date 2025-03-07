/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../../../inc/MarlinConfigPre.h"
#include "../../../feature/bedlevel/bedlevel.h"

class LevelingBilinear {
  private:
  static xy_pos_t grid_spacing, grid_start;
  static xy_float_t grid_factor;
  static bed_mesh_t z_values;
  static xy_pos_t cached_rel;
  static xy_int8_t cached_g;

  static void extrapolate_one_point(const uint8_t x, const uint8_t y, const int8_t xdir, const int8_t ydir);

  #if ENABLED(ABL_BILINEAR_SUBDIVISION)
    #define ABL_GRID_POINTS_VIRT_X (GRID_MAX_CELLS_X * (BILINEAR_SUBDIVISIONS) + 1)
    #define ABL_GRID_POINTS_VIRT_Y (GRID_MAX_CELLS_Y * (BILINEAR_SUBDIVISIONS) + 1)

    static float z_values_virt[ABL_GRID_POINTS_VIRT_X][ABL_GRID_POINTS_VIRT_Y];
    static xy_pos_t grid_spacing_virt;
    static xy_float_t grid_factor_virt;

    static float bed_level_virt_coord(const uint8_t x, const uint8_t y);
    static float bed_level_virt_cmr(const float p[4], const uint8_t i, const float t);
    static float bed_level_virt_2cmr(const uint8_t x, const uint8_t y, const_float_t tx, const_float_t ty);
    static void bed_level_virt_interpolate();
  #endif

public:
  static void reset();
  static void set_grid(const xy_pos_t& _grid_spacing, const xy_pos_t& _grid_start);
  static void extrapolate_unprobed_bed_level();
  static void print_leveling_grid(const bed_mesh_t* _z_values = NULL);
  static void refresh_bed_level();
  static bool has_mesh() { return !!grid_spacing.x; }
  static bed_mesh_t& get_z_values() { return z_values; }
  static const xy_pos_t& get_grid_spacing() { return grid_spacing; }
  static const xy_pos_t& get_grid_start() { return grid_start; }
  static float get_mesh_x(int16_t i) { return ((grid_start.x +25)+ i * grid_spacing.x); }
  static float get_mesh_y(int16_t j) { return ((grid_start.y -25) + j * grid_spacing.y); }
  static float get_z_correction(const xy_pos_t &raw);

  #if IS_CARTESIAN && DISABLED(SEGMENT_LEVELED_MOVES)
    static void line_to_destination(const_feedRate_t scaled_fr_mm_s, uint16_t x_splits=0xFFFF, uint16_t y_splits=0xFFFF);
  #endif
};

extern LevelingBilinear bbl;

//#define _GET_MESH_X(I) bbl.get_mesh_x(I)
//#define _GET_MESH_Y(J) bbl.get_mesh_y(J)
//#define Z_VALUES_ARR bbl.get_z_values()
