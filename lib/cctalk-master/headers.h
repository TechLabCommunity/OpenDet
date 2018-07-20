// just the definitions for cctalk headers
	#define reset_device 1
	#define request_comms_status_variables 2
	#define clear_comms_status_variables 3
	#define request_comms_revision 4
	#define request_service_status 104
	#define data_stream 105
	#define request_escrow_status 106
	#define operate_escrow 107
	#define request_encrypted_monetary_id 108
	#define request_encrypted_hopper_status 109
	#define switch_encryption_key 110
	#define request_encryption_support 111
	#define read_encrypted_events 112
	#define switch_baud_rate 113
	#define request_USB id 114
	#define request_real_time_clock 115
	#define modify_real_time_clock 116
	#define request_cashbox_value 117
	#define modify_cashbox_value 118
	#define request_hopper_balance 119
	#define modify_hopper_balance 120
	#define purge_hopper 121
	#define request_error_status 122
	#define request_activity_register 123
	#define verify_money_out 124
	#define pay_money_out 125
	#define clear_money_counter 126
	#define request money_out 127
	#define request_money_in 128
	#define read_barcode_data 129
	#define request_indexed_hopper_dispense_count 130
	#define request_hopper_coin_value 131
	#define emergency_stop_value 132
	#define request_hopper_polling_value 133
	#define dispense_hopper_value 134
	#define set_accept_limit 135
	#define store_encryption_code 136
	#define switch_encryption_code 137
	#define finish_firmware_upgrade 138
	#define begin_firmware_upgrade 139
	#define upload_firmware 140
	#define request_firmware_upgrade_capability 141
	#define finish_bill_table_upgrade 142
	#define begin_bill_table_upgrade 143
	#define upload_bill_tables 144
	#define request_currency_revision 145
	#define operate_bidirectional_motors 146
	#define perform_stacker_cycle 147
	#define read_opto_voltages 148
	#define request_individual_error_counter 149
	#define request_individual_accept_counter 150
	#define test_lamps 151
	#define request_bill_operating_mode 152
	#define modify_bill_operating_mode 153
	#define route_bill 154
	#define request_bill_position 155
	#define request_country_scaling_factor 156
	#define request_bill_id 157
	#define modify_bill_id 158
	#define read_buffered_bill_events 159
	#define request_cipher_key 160
	#define pump_rng 161
	#define modify_inhibit_and_override_registers 162
	#define test_hopper 163
	#define enable_hopper 164
	#define modify_variable_set 165
	#define request_hopper_status 166
	#define dispense_hopper_coins 167
	#define request_hopper_dispense_count 168
	#define request_address_mode 169
	#define request_base_year 170
	#define request_hopper_coin 171
	#define emergency_stop 172
	#define request_thermistor_reading 173
	#define request_payout_float 174
	#define modify_payout_float 175
	#define request_alarm_counter 176
	#define handheld_function 177
	#define request_bank_select 178
	#define modify_bank_select 179
	#define request_security_setting 180
	#define modify_security_setting 181
	#define download_calibration_info 182
	#define upload_window_data 183
	#define request_coin_id 184
	#define modify_coin_id 185
	#define request_payout_capacity 186
	#define modify_payout_capacity 187
	#define request_default_sorter_path 188
	#define modify_default_sorter_path 189
	#define keypad_control 191
	#define request_build_code 192
	#define request_fraud_counter 193
	#define request_reject_counter 194
	#define request_last_modification_date 195
	#define request_creation_date 196
	#define calculate_rom_checksum 197
	#define counters_to_eeprom 198
	#define configuration_to_eeprom 199
	#define acmi_unencrypted_product_id 200
	#define request_teach_status 201
	#define teach_mode_control 202
	#define display_control 203
	#define meter_control 204
	#define request_payout_absolute_count 207
	#define modify_payout_absolute_count 208
	#define request_sorter_paths 209
	#define modify_sorter_paths 210
	#define power_management_control 211
	#define request_coin_position 212
	#define request_option_flags 213
	#define write_data_block 214
	#define read_data_block 215
	#define request_data_storage_availability 216
	#define request_payout_highlow_status 217
	#define enter_pin_number 218
	#define enter_new_pin_number 219
	#define acmi_encrypted_data 220
	#define request_sorter_override_status 221
	#define modify_sorter_override_status 222
	#define modify_encrypted_inhibit_and_override_registers 223
	#define request_encrypted_product_id 224
	#define request_accept_counter 225
	#define request_insertion_counter 226
	#define request_master_inhibit_status 227
	#define modify_master_inhibit_status 228
	#define read_buffered_credit_or_error_codes 229
	#define request_inhibit_status 230
	#define modify_inhibit_status 231
	#define perform_self_check 232
	#define latch_output_lines 233
	#define send_dh_public_key 234
	#define read_dh_public_key 235
	#define read_opto_states 236
	#define read_input_lines 237
	#define test_output_lines 238
	#define operate_motors 239
	#define test_solenoids 240
	#define request_software_revision 241
	#define request_serial_number 242
	#define request_database_version 243
	#define request_product_code 244
	#define request_equipment_category_id 245
	#define request_manufacturer_id 246
	#define request_variable_set 247
	#define request_status 248
	#define request_polling_priority 249
	#define address_random 250
	#define address_change 251
	#define address_clash 252
	#define address_poll 253
	#define simple_poll 254
	#define factory_setup_and_test 255

