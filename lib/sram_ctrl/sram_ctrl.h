#pragma once

enum SRAM_STATE {SRAM_UNDEFINED, SRAM_ACTIVE, SRAM_INACTIVE};
extern enum SRAM_STATE g_sram_active;

enum SRAM_STATE is_sram_active ();
void _sram_set_active ();
void _sram_set_deactive();
void _sram_set_undefined ();

void get_sram_state (char *);

void activate_sram ();
void deactivate_sram ();