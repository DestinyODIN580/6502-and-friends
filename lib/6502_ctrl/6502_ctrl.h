#pragma once

enum PROC_STATE {PROC_UNDEFINED, PROC_ACTIVE, PROC_INACTIVE};
extern enum PROC_STATE g_6502_active;

enum PROC_STATE is_6502_active ();
void _6502_set_active ();
void _6502_set_deactive();
void _6502_set_undefined ();

void init_6502 ();
void activate_6502 ();
void deactivate_6502 ();
void destroy_6502 ();