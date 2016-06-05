/*
Copyright (C) 2008-2016 Vana Development Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#pragma once

#include "Common/Types.hpp"

namespace vana {
	namespace parties {
		const size_t max_members = 6;
	}

	namespace characters {
		const size_t min_name_size = 4;
		const size_t max_name_size = 12;
		const size_t min_password_size = 5;
		const size_t max_password_size = 15;
		const int32_t default_character_slots = 3;
	}

	namespace gender {
		enum : game_gender_id {
			male = 0,
			female = 1,
			both = 2
		};
	}

	namespace buddies {
		const size_t max_group_name_size = 13;
	}

	namespace guilds_and_alliances {
		const size_t min_name_size = 4;
		const size_t max_name_size = 12;
		const int32_t rank_quantity = 5;
		const int32_t max_guild_capacity = 100;
		const int32_t max_alliance_capacity = 3;
		const int32_t threads_per_page = 10;
	}

	namespace stats {
		const game_player_level player_levels = 200;
		const game_player_level cygnus_levels = 120;
		const uint8_t pet_levels = 30;
		const uint8_t mount_levels = 30;
		const game_health max_max_hp = 30000;
		const game_health min_max_hp = 1;
		const game_health max_max_mp = 30000;
		const game_health min_max_mp = 1;
		const game_fame max_fame = 30000;
		const game_fame min_fame = -30000;
		const game_health min_hp = 0;
		const game_health min_mp = 0;
		const game_health default_hp = 50;
		const int16_t max_closeness = 30000;
		const game_stat ap_per_level = 5;
		const game_stat ap_per_cygnus_level = 6;
		const game_player_level cygnus_ap_cutoff = 70;
		const game_stat sp_per_level = 3;
		const int8_t max_fullness = 100;
		const int8_t min_fullness = 0;
		const int8_t pet_feed_fullness = 30;
		const game_damage max_damage = 199999;
		const int16_t energy_charge_decay = 200;
		const int16_t max_energy_charge_level = 10000;

		const int16_t pet_exp[pet_levels - 1] = {
			1, 3, 6, 14, 31, 60, 108, 181, 287, 434,
			632, 891, 1224, 1642, 2161, 2793, 3557, 4467, 5542, 6801,
			8263, 9950, 11882, 14084, 16578, 19391, 22548, 26074, 30000
		};

		const int16_t mount_exp[mount_levels - 1] = {
			6, 25, 50, 105, 134, 196, 254, 263, 315, 367,
			430, 543, 587, 679, 725, 897, 1146, 1394, 1701, 2247,
			2543, 2898, 3156, 3313, 3584, 3923, 4150, 4305, 4550
		};

		const game_experience player_exp[player_levels - 1] = {
			// 20 rows of 10 levels each (with 9 in the last row)

	#if MAPLE_LOCALE == MAPLE_LOCALE_GLOBAL
	#	if MAPLE_VERSION < 70 /* Original EXP curve */
			15, 34, 57, 92, 135, 372, 560, 840, 1242, 1716,
			2360, 3216, 4200, 5460, 7050, 8840, 11040, 13716, 16680, 20216,
			24402, 28980, 34320, 40512, 47216, 54900, 63666, 73080, 83720, 95700,
			108480, 122760, 138666, 155540, 174216, 194832, 216600, 240500, 266682, 294216,
			324240, 356916, 391160, 428280, 468450, 510420, 555680, 604416, 655200, 709716,
			748608, 789631, 832902, 878545, 926689, 977471, 1031036, 1087536, 1147132, 1209994,
			1276301, 1346242, 1420016, 1497832, 1579913, 1666492, 1757815, 1854143, 1955750, 2062925,
			2175973, 2295216, 2420993, 2553663, 2693603, 2841212, 2996910, 3161140, 3334370, 3517093,
			3709829, 3913127, 4127566, 4353756, 4592341, 4844001, 5109452, 5389449, 5684790, 5996316,
			6324914, 6671519, 7037118, 7422752, 7829518, 8258575, 8711144, 9188514, 9692044, 10223168,
			10783397, 11374327, 11997640, 12655110, 13348610, 14080113, 14851703, 15665576, 16524049, 17429566,
			18384706, 19392187, 20454878, 21575805, 22758159, 24005306, 25320796, 26708375, 28171993, 29715818,
			31344244, 33061908, 34873700, 36784778, 38800583, 40926854, 43169645, 45535341, 48030677, 50662758,
			53439077, 56367538, 59456479, 62714694, 66151459, 69776558, 73600313, 77633610, 81887931, 86375389,
			91108760, 96101520, 101367883, 106992842, 112782213, 118962678, 125481832, 132358236, 139611467, 147262175,
			155332142, 163844343, 172823012, 182293713, 192283408, 202820538, 213935103, 225658746, 238024845, 251068606,
			264827165, 279339639, 294647508, 310794191, 327825712, 345790561, 364739883, 384727628, 405810702, 428049128,
			451506220, 476248760, 502347192, 529875818, 558913012, 589541445, 621848316, 655925603, 691870326, 729784819,
			769777027, 811960808, 856456260, 903390063, 952895838, 1005114529, 1060194805, 1118293480, 1179575962, 1244216724,
			1312399800, 1384319309, 1460180007, 1540197871, 1624600714, 1713628833, 1807535693, 1906558648, 2011069705
	#	elif MAPLE_VERSION < 93 /* EXP curve .70-.92 */
			15, 34, 57, 92, 135, 372, 560, 840, 1242, 1144,
			1573, 2144, 2800, 3640, 4700, 5893, 7360, 9144, 11120, 13478,
			16268, 19320, 22881, 27009, 31478, 36601, 42446, 48722, 55816, 76560,
			86784, 98208, 110932, 124432, 139372, 155865, 173280, 192400, 213345, 235372,
			259392, 285532, 312928, 342624, 374760, 408336, 444544, 483532, 524160, 567772,
			598886, 631704, 666321, 702836, 741351, 781976, 824828, 870028, 917705, 967995,
			1021040, 1076993, 1136012, 1198265, 1263930, 1333193, 1406252, 1483314, 1564600, 1650340,
			1740778, 1836172, 1936794, 2042930, 2154882, 2272969, 2397528, 2528912, 2667496, 2813674,
			2967863, 3130501, 3302052, 3483004, 3673872, 3875200, 4087561, 4311559, 4547832, 4797052,
			5059931, 5337215, 5629694, 5938201, 6263614, 6606860, 6968915, 7350811, 7753635, 8178534,
			8626717, 9099461, 9598112, 10124088, 10678888, 11264090, 11881362, 12532460, 13219239, 13943652,
			14707764, 15513749, 16363902, 17260644, 18206527, 19204244, 20256636, 21366700, 22537594, 23772654,
			25075395, 26449526, 27898960, 29427822, 31040466, 32741483, 34535716, 36428272, 38424541, 40530206,
			42751261, 45094030, 47565183, 50171755, 52921167, 55821246, 58880250, 62106888, 65510344, 69100311,
			72887008, 76881216, 81094306, 85538273, 90225770, 95170142, 100385465, 105886588, 111689173, 117809740,
			124265713, 131075474, 138258409, 145834970, 153826726, 162256430, 171148082, 180526996, 190419876, 200854884,
			211861732, 223471754, 235718006, 248635352, 262260569, 276632448, 291791906, 307782102, 324648561, 342439302,
			361204976, 380999008, 401877753, 423900654, 447130409, 471633156, 497478652, 524740482, 553496260, 583827855,
			615821621, 649568646, 685165008, 722712050, 762316670, 804091623, 848155844, 894634784, 943660769, 995373379,
			1049919840, 1107455447, 1168144005, 1232158296, 1299680571, 1370903066, 1446028554, 1525270918, 1608855764
	#	else /* EXP curve .93+ */
			15, 34, 57, 92, 135, 372, 560, 840, 1242, 1242,
			1242, 1242, 1242, 1242, 1490, 1788, 2146, 2575, 3090, 3708,
			4450, 5340, 6408, 7690, 9228, 11074, 13289, 15947, 19136, 19136,
			19136, 19136, 19136, 19136, 22963, 27556, 33067, 39681, 47616, 51425,
			55539, 59582, 64781, 69963, 75560, 81605, 88133, 95184, 102799, 111023,
			119905, 129497, 139857, 151046, 163129, 176180, 190274, 205496, 221936, 239691,
			258866, 279575, 301941, 326097, 352184, 380359, 410788, 443651, 479143, 479143,
			479143, 479143, 479143, 479143, 512683, 548571, 586971, 628059, 672024, 719065,
			769400, 823258, 880886, 942548, 1008526, 1079123, 1154662, 1235488, 1321972, 1414511,
			1513526, 1619473, 1732836, 1854135, 1983924, 2122799, 2271395, 2430393, 2600520, 2782557,
			2977336, 3185749, 3408752, 3647365, 3902680, 4175868, 4468179, 4780951, 5115618, 5473711,
			5856871, 6266852, 6705531, 7176919, 7677163, 8214565, 8789584, 9404855, 10063195, 10063195,
			10063195, 10063195, 10063195, 10063195, 10767619, 11521352, 12327847, 13190796, 14114152, 15102142,
			16159292, 17290443, 18500774, 19795828, 21181536, 22664244, 24250741, 25948292, 27764673, 29708200,
			31787774, 34012918, 36393823, 38941390, 41667288, 44583998, 47704878, 51044219, 54617315, 58440527,
			62531364, 66908559, 71592158, 76603609, 81965862, 87703472, 93842715, 100411706, 107440525, 113895024,
			120728726, 127972450, 135650797, 143789844, 152417235, 161565269, 171256005, 181531366, 192423248, 203968643,
			216206761, 229179167, 242929917, 257505712, 272956055, 289333418, 306693423, 525095029, 344600730, 365276774,
			387193381, 410424983, 435050483, 461153512, 488822722, 518152086, 549241211, 582195683, 617127424, 654155070,
			693404374, 735008637, 779109155, 825855704, 875407047, 927931469, 983607358, 1042623799, 1105181227
	#	endif
	#else
	#	error Please fill in appropriate EXP levels for your locale/version
	#endif
		};

		namespace base_hp {
			const game_health variation = 4; // This is the range of HP that the server will give

			const game_health beginner = 12; // These are base HP values rewarded on level up
			const game_health warrior = 24;
			const game_health magician = 10;
			const game_health bowman = 20;
			const game_health thief = 20;
			const game_health pirate = 22;
			const game_health gm = 150;

			const game_health beginner_ap = 8; // These are base HP values rewarded on AP distribution
			const game_health warrior_ap = 20;
			const game_health magician_ap = 8;
			const game_health bowman_ap = 16;
			const game_health thief_ap = 16;
			const game_health pirate_ap = 18;
			const game_health gm_ap = 16;
		}
		namespace base_mp {
			const game_health variation = 2; // This is the range of MP that the server will give

			const game_health beginner = 10; // These are base MP values rewarded on level up
			const game_health warrior = 4;
			const game_health magician = 6;
			const game_health bowman = 14;
			const game_health thief = 14;
			const game_health pirate = 18;
			const game_health gm = 150;

			const game_health beginner_ap = 6; // These are base MP values rewarded on AP distribution
			const game_health warrior_ap = 2;
			const game_health magician_ap = 18;
			const game_health bowman_ap = 10;
			const game_health thief_ap = 10;
			const game_health pirate_ap = 14;
			const game_health gm_ap = 10;
		}
		enum constants : int32_t {
			skin = 0x01,
			face = 0x02,
			hair = 0x04,
			pet = 0x08,
			level = 0x10,
			job = 0x20,
			str = 0x40,
			dex = 0x80,
			intl = 0x100,
			luk = 0x200,
			hp = 0x400,
			max_hp = 0x800,
			mp = 0x1000,
			max_mp = 0x2000,
			ap = 0x4000,
			sp = 0x8000,
			exp = 0x10000,
			fame = 0x20000,
			mesos = 0x40000,
		};
	}

	namespace monster_cards {
		const uint8_t max_card_level = 5;
		const int32_t max_player_level = 8;
		const int32_t player_levels[max_player_level - 1] = {10, 30, 60, 100, 150, 210, 280};
	}

	enum class mob_elemental_attribute : int8_t {
		normal,
		immune,
		strong,
		weak,
	};
}