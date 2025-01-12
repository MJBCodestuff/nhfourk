/* vim:set cin ft=c sw=4 sts=4 ts=8 et ai cino=Ls\:0t0(0 : -*- mode:c;fill-column:80;tab-width:8;c-basic-offset:4;indent-tabs-mode:nil;c-file-style:"k&r" -*-*/
/* Last modified by Alex Smith, 2015-03-13 */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985-1999. */
/* NetHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "hungerstatus.h"

static boolean ok_race(int, int, int, int);
static boolean ok_gend(int, int, int, int);
static boolean ok_align(int, int, int, int);
static boolean only_sylph_safe_armor(struct monst *, enum objslot);


/*** Table of all roles ***/
/* According to AD&D, HD for some classes (ex. Wizard) should be smaller
 * (4-sided for wizards).  But this is not AD&D, and using the AD&D
 * rule here produces an unplayable character.  Thus I have used a minimum
 * of an 10-sided hit die for everything.  Another AD&D change: wizards get
 * a minimum strength of 4 since without one you can't teleport or cast
 * spells. --KAA
 *
 * As the wizard has been updated (wizard patch 5 jun '96) their HD can be
 * brought closer into line with AD&D. This forces wizards to use magic more
 * and distance themselves from their attackers. --LSZ
 *
 * With the introduction of races, some hit points and energy
 * has been reallocated for each race.  The values assigned
 * to the roles has been reduced by the amount allocated to
 * humans.  --KMH
 *
 * God names use a leading underscore to flag goddesses.
 */
const struct Role roles[] = {
    {{"Archeologist", 0},
     "Quetzalcoatl", "Camaxtli", "Huhetotl",    /* Central American */
     "Arc", "the College of Archeology", "the Tomb of the Toltec Kings",
     PM_ARCHEOLOGIST, NON_PM, NON_PM,
     PM_LORD_CARNARVON, PM_STUDENT, PM_MINION_OF_HUHETOTL,
     NON_PM, PM_HUMAN_MUMMY, S_SNAKE, S_MUMMY,
     ART_ORB_OF_DETECTION,
     MRACE_HUMAN | MRACE_DWARF | MRACE_GNOME | MRACE_SCURRIER |
     ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL,
     /* Str Int Wis Dex Con Cha */
     {7, 10, 10, 7, 7, 7},
     {20, 20, 20, 10, 20, 10},
     {1, 1, 1, 1, 1, 1},
     /* Init Lower Higher */
     {11, 0, 0, 8, 1, 0},       /* Hit points */
     {1, 0, 0, 1, 0, 1}, 14,    /* Energy */
     10, 5, 0, 2, 10, A_INT, SPE_MAGIC_MAPPING, -4, achieve_quest_archeologist,
     UNLOCKFEAT_ROLE_ARCHEOLOGIST, UNLOCKFEAT_ROLE_ARCHEOLOGIST},
    /* The fact that Barbarian is at index 1 in this list is mildly significant
       to player_selection, for pre-initializing randrole. */
    {{"Barbarian", 0},
     "Mitra", "Crom", "Set",    /* Hyborian */
     "Bar", "the Camp of the Duali Tribe", "the Duali Oasis",
     PM_BARBARIAN, NON_PM, NON_PM,
     PM_PELIAS, PM_CHIEFTAIN, PM_THOTH_AMON,
     PM_OGRE, PM_TROLL, S_OGRE, S_TROLL,
     ART_HEART_OF_AHRIMAN,
     MRACE_HUMAN | MRACE_DWARF | MRACE_ORC | MRACE_GIANT
              | ROLE_MALE | ROLE_FEMALE
              | ROLE_LAWFUL | ROLE_NEUTRAL | ROLE_CHAOTIC,
     /* Str Int Wis Dex Con Cha */
     {16, 7, 7, 15, 16, 6},
     {30, 6, 7, 20, 30, 7},
     {2, 0, 0, 2, 2, 0},
     /* Init Lower Higher */
     {14, 0, 0, 10, 2, 0},      /* Hit points */
     {1, 0, 0, 1, 0, 1}, 10,    /* Energy */
     10, 15, 0, 0, 8, A_INT, SPE_HASTE_SELF, -4, achieve_quest_barb,
     UNLOCKFEAT_ROLE_BARBARIAN, UNLOCKFEAT_ALLROLES_BARB},
    {{"Caveman", "Cavewoman"},
     "Anu", "_Ishtar", "Anshar",        /* Babylonian */
     "Cav", "Home Cave", "the Dragon's Lair",
     PM_CAVEMAN, PM_CAVEWOMAN, PM_PUPPY,
     PM_SHAMAN_KARNOV, PM_NEANDERTHAL, PM_GREAT_FIERCE_BEAST,
     PM_BUGBEAR, PM_HILL_GIANT, S_HUMANOID, S_GIANT,
     ART_BIG_STICK,
     MRACE_HUMAN | MRACE_ORC | MRACE_GIANT |
     ROLE_MALE | ROLE_FEMALE | ROLE_CHAOTIC | ROLE_NEUTRAL,
     /* Str Int Wis Dex Con Cha */
     {10, 7, 7, 7, 8, 6},
     {30, 6, 7, 20, 30, 7},
     {3, 0, 0, 1, 1, 1},
     /* Init Lower Higher */
     {14, 0, 0, 8, 2, 0},       /* Hit points */
     {1, 0, 0, 1, 0, 1}, 10,    /* Energy */
     10, 12, 0, 1, 8, A_INT, SPE_DIG, -4, achieve_quest_caveman,
     UNLOCKFEAT_ROLE_CAVEMAN, UNLOCKFEAT_ROLE_CAVEMAN},
    {{"Healer", 0},
     "_Athena", "Hermes", "Poseidon",   /* Greek */
     "Hea", "the Temple of Epidaurus", "the Temple of Coeus",
     PM_HEALER, NON_PM, NON_PM,
     PM_HIPPOCRATES, PM_ATTENDANT, PM_CYCLOPS,
     PM_GIANT_RAT, PM_PIT_VIPER, S_RODENT, S_YETI,
     ART_STAFF_OF_AESCULAPIUS,
     MRACE_HUMAN | MRACE_GNOME | MRACE_SYLPH | ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL,
     /* Str Int Wis Dex Con Cha */
     {7, 7, 13, 7, 11, 16},
     {15, 20, 20, 15, 25, 5},
     {0, 1, 2, 1, 1, 1},
     /* Init Lower Higher */
     {11, 0, 0, 8, 1, 0},       /* Hit points */
     {1, 4, 0, 1, 0, 2}, 20,    /* Energy */
     10, 3, -3, 2, 10, A_INT, SPE_CURE_SICKNESS, -4, achieve_quest_healer,
     UNLOCKFEAT_ROLE_HEALER, UNLOCKFEAT_ROLE_HEALER},
    {{"Hoplite", "Shieldmaiden"},
     "Tyr", "Odin", "Loki",     /* Norse */
     "Shi", "the Shrine of Destiny", "the cave of Surtur",
     PM_HOPLITE, PM_SHIELDMAIDEN, PM_PUPPY, /* Valks are special cased. */
     PM_NORN, PM_WARRIOR, PM_LORD_SURTUR,
     PM_FIRE_ANT, PM_FIRE_GIANT, S_ANT, S_GIANT,
     ART_ORB_OF_FATE,
     MRACE_HUMAN | MRACE_DWARF | MRACE_GIANT | MRACE_VALKYRIE |
     ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL,
     /* Str Int Wis Dex Con Cha */
     {10, 7, 7, 7, 10, 7},
     {30, 6, 7, 20, 30, 7},
     {1, 1, 1, 2, 1, 0},
     /* Init Lower Higher */
     {14, 0, 0, 8, 2, 0},       /* Hit points */
     {1, 0, 0, 1, 0, 1}, 10,    /* Energy */
     10, 10, -2, 0, 9, A_INT, SPE_CONE_OF_COLD, -4, achieve_quest_shieldmaiden,
     UNLOCKFEAT_ROLE_SHIELDMAIDEN, UNLOCKFEAT_ROLE_SHIELDMAIDEN},
    {{"Knight", 0},
     "Lugh", "_Brigit", "Manannan Mac Lir",     /* Celtic */
     "Kni", "Camelot Castle", "the Isle of Glass",
     PM_KNIGHT, NON_PM, PM_PONY,
     PM_KING_ARTHUR, PM_PAGE, PM_IXOTH,
     PM_QUASIT, PM_OCHRE_JELLY, S_IMP, S_JELLY,
     ART_MAGIC_MIRROR_OF_MERLIN,
     MRACE_HUMAN | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL,
     /* Str Int Wis Dex Con Cha */
     {13, 7, 14, 8, 10, 17},
     {30, 15, 15, 10, 20, 10},
     {2, 2, 0, 0, 0, 2},
     /* Init Lower Higher */
     {14, 0, 0, 8, 2, 0},       /* Hit points */
     {1, 4, 0, 1, 0, 2}, 10,    /* Energy */
     10, 8, -2, 0, 9, A_INT, SPE_TURN_UNDEAD, -4, achieve_quest_knight,
     UNLOCKFEAT_ROLE_KNIGHT, UNLOCKFEAT_ROLE_KNIGHT},
    {{"Monk", 0},
     "Shan Lai Ching", "Chih Sung-tzu", "Huan Ti",      /* Chinese */
     "Mon", "the Monastery of Chan-Sune",
     "the Monastery of the Earth-Lord",
     PM_MONK, NON_PM, NON_PM,
     PM_GRAND_MASTER, PM_ABBOT, PM_MASTER_KAEN,
     PM_EARTH_ELEMENTAL, PM_XORN, S_ELEMENTAL, S_XORN,
     ART_EYES_OF_THE_OVERWORLD,
     MRACE_HUMAN | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL |
     ROLE_CHAOTIC,
     /* Str Int Wis Dex Con Cha */
     {10, 7, 8, 8, 7, 7},
     {25, 10, 20, 20, 15, 10},
     {2, 1, 2, 1, 0, 0},
     /* Init Lower Higher */
     {12, 0, 0, 8, 1, 0},       /* Hit points */
     {2, 2, 0, 2, 0, 2}, 10,    /* Energy */
     10, 8, -2, 2, 20, A_INT, SPE_RESTORE_ABILITY, -4, achieve_quest_monk,
     UNLOCKFEAT_ROLE_MONK, UNLOCKFEAT_ROLE_MONK},
    {{"Priest", "Priestess"},
     0, 0, 0,   /* chosen randomly from among the other roles */
     "Pri", "the Great Temple", "the Temple of Nalzok",
     PM_PRIEST, PM_PRIESTESS, NON_PM,
     PM_ARCH_PRIEST, PM_ACOLYTE, PM_NALZOK,
     PM_HUMAN_ZOMBIE, PM_WRAITH, S_ZOMBIE, S_WRAITH,
     ART_SCEPTRE_OF_MIGHT,
     MRACE_HUMAN | MRACE_ELF | MRACE_DWARF | MRACE_SYLPH | MRACE_VALKYRIE
              | ROLE_MALE | ROLE_FEMALE
              | ROLE_LAWFUL | ROLE_NEUTRAL | ROLE_CHAOTIC,
     /* Str Int Wis Dex Con Cha */
     {7, 10, 7, 7, 7, 7},
     {15, 30, 10, 15, 20, 10},
     {1, 2, 0, 1, 2, 0},
     /* Init Lower Higher */
     {12, 0, 0, 8, 1, 0},       /* Hit points */
     {4, 3, 0, 2, 0, 2}, 10,    /* Energy */
     10, 3, -2, 2, 10, A_INT, SPE_REMOVE_CURSE, -4,
     achieve_quest_priest, UNLOCKFEAT_ROLE_PRIEST, UNLOCKFEAT_ROLE_PRIEST},
    /* Note: Rogue precedes Ranger so that use of `-R' on the command line
       retains its traditional meaning. */
    {{"Rogue", 0},
     "Issek", "Mog", "Kos",     /* Nehwon */
     "Rog", "the Thieves' Guild Hall", "the Assassins' Guild Hall",
     PM_ROGUE, NON_PM, NON_PM,
     PM_MASTER_OF_THIEVES, PM_THUG, PM_MASTER_ASSASSIN,
     PM_LEPRECHAUN, PM_GUARDIAN_NAGA, S_NYMPH, S_NAGA,
     ART_MASTER_KEY_OF_THIEVERY,
     MRACE_HUMAN | MRACE_ORC | MRACE_ELF | MRACE_SYLPH | ROLE_MALE | ROLE_FEMALE | ROLE_CHAOTIC,
     /* Str Int Wis Dex Con Cha */
     {7, 7, 7, 10, 7, 6},
     {20, 10, 10, 30, 20, 10},
     {1, 1, 0, 2, 1, 1},
     /* Init Lower Higher */
     {10, 0, 0, 8, 1, 0},       /* Hit points */
     {1, 0, 0, 1, 0, 1}, 11,    /* Energy */
     10, 8, 0, 1, 9, A_INT, SPE_DETECT_TREASURE, -4,
     achieve_quest_rogue, UNLOCKFEAT_ROLE_ROGUE, UNLOCKFEAT_ROLE_ROGUE},
    {{"Ranger", 0},
     "Mercury", "_Venus", "Mars",       /* Roman/planets */
     "Ran", "Orion's camp", "the cave of the wumpus",
     PM_RANGER, NON_PM, PM_PUPPY /* Orion & canis major */ ,
     PM_ORION, PM_HUNTER, PM_SCORPIUS,
     PM_FOREST_CENTAUR, PM_SCORPION, S_CENTAUR, S_SPIDER,
     ART_LONGBOW_OF_DIANA,
     MRACE_HUMAN | MRACE_ELF | MRACE_GNOME | MRACE_ORC | MRACE_SYLPH |
     MRACE_GIANT | MRACE_SCURRIER |
     ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL | ROLE_CHAOTIC,
     /* Str Int Wis Dex Con Cha */
     {13, 13, 13, 9, 13, 7},
     {30, 10, 10, 20, 20, 10},
     {1, 1, 1, 2, 0, 1},
     /* Init Lower Higher */
     {13, 0, 0, 6, 1, 0},       /* Hit points */
     {1, 0, 0, 1, 0, 1}, 12,    /* Energy */
     10, 9, 2, 1, 10, A_INT, SPE_INVISIBILITY, -4,
     achieve_quest_ranger, UNLOCKFEAT_ROLE_RANGER, UNLOCKFEAT_ROLE_RANGER},
    {{"Samurai", 0},
     "_Amaterasu Omikami", "Raijin", "Susanowo",        /* Japanese */
     "Sam", "the Castle of the Taro Clan", "the Shogun's Castle",
     PM_SAMURAI, NON_PM, PM_PUPPY,
     PM_LORD_SATO, PM_ROSHI, PM_ASHIKAGA_TAKAUJI,
     PM_WOLF, PM_STALKER, S_DOG, S_ELEMENTAL,
     ART_TSURUGI_OF_MURAMASA,
     MRACE_HUMAN | MRACE_GIANT | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL,
     /* Str Int Wis Dex Con Cha */
     {10, 8, 7, 10, 17, 6},
     {30, 10, 8, 30, 14, 8},
     {2, 0, 1, 2, 1, 0},
     /* Init Lower Higher */
     {13, 0, 0, 8, 1, 0},       /* Hit points */
     {1, 0, 0, 1, 0, 1}, 11,    /* Energy */
     10, 10, 0, 0, 8, A_INT, SPE_CLAIRVOYANCE, -4,
     achieve_quest_samurai, UNLOCKFEAT_ROLE_SAMURAI, UNLOCKFEAT_ROLE_SAMURAI},
    {{"Tourist", 0},
     "Blind Io", "_The Lady", "Offler", /* Discworld */
     "Tou", "Ankh-Morpork", "the Thieves' Guild Hall",
     PM_TOURIST, NON_PM, NON_PM,
     PM_TWOFLOWER, PM_GUIDE, PM_MASTER_OF_THIEVES,
     PM_GIANT_SPIDER, PM_FOREST_CENTAUR, S_SPIDER, S_CENTAUR,
     ART_YENDORIAN_EXPRESS_CARD,
     MRACE_HUMAN | MRACE_GNOME | MRACE_SCURRIER |
     ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL,
     /* Str Int Wis Dex Con Cha */
     {7, 10, 6, 7, 7, 10},
     {15, 10, 10, 15, 30, 20},
     {0, 1, 2, 1, 0, 2},
     /* Init Lower Higher */
     {8, 0, 0, 8, 0, 0},        /* Hit points */
     {1, 0, 0, 1, 0, 1}, 14,    /* Energy */
     10, 5, 1, 2, 10, A_INT, SPE_CHARM_MONSTER, -4,
     achieve_quest_tourist, UNLOCKFEAT_ROLE_TOURIST, UNLOCKFEAT_ROLE_TOURIST},
    {{"Wizard", 0},
     "Ptah", "Thoth", "Anhur",  /* Egyptian */
     "Wiz", "the Lonely Tower", "the Tower of Darkness",
     PM_WIZARD, NON_PM, PM_KITTEN,
     PM_NEFERET_THE_GREEN, PM_APPRENTICE, PM_DARK_ONE,
     PM_VAMPIRE_BAT, PM_XORN, S_BAT, S_WRAITH,
     ART_EYE_OF_THE_AETHIOPICA,
     MRACE_HUMAN | MRACE_ELF | MRACE_GNOME | MRACE_ORC | MRACE_SYLPH |
     ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL | ROLE_CHAOTIC,
     /* Str Int Wis Dex Con Cha */
     {6, 10, 10, 6, 7, 6},
     {10, 25, 25, 15, 15, 10},
     {0, 2, 2, 1, 0, 1},
     /* Init Lower Higher */
     {10, 0, 0, 8, 1, 0},       /* Hit points */
     {4, 3, 0, 2, 0, 3}, 12,    /* Energy */
     10, 1, 0, 3, 10, A_INT, SPE_MAGIC_MISSILE, -4,
     achieve_quest_wizard, UNLOCKFEAT_ROLE_WIZARD, UNLOCKFEAT_ALLROLES_WIZARD},
/* Array terminator */
    {{0, 0}}
};


/* The player's role, created at runtime from initial
 * choices.  This will be munged in role_init().
 */
struct Role urole;



/* Table of all races */
const struct Race races[] = {
    {"human", "human", "humanity", "Hum",
     {"man", "woman"},
     12, /* base speed, normal */
     PM_HUMAN, NON_PM, PM_HUMAN_MUMMY, PM_HUMAN_ZOMBIE,
     MRACE_HUMAN | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL |
     ROLE_CHAOTIC,
     MRACE_HUMAN, 0, MRACE_GNOME | MRACE_ORC,
     /* Str Int Wis Dex Con Cha */
     {3, 3, 3, 3, 3, 3},
     {STR18(100), 18, 18, 18, 18, 18},
     /* Init Lower Higher */
     {2, 0, 0, 2, 1, 0},        /* Hit points */
     {1, 0, 2, 0, 2, 0},        /* Energy */
     UNLOCKFEAT_RACE_HUMAN, UNLOCKFEAT_ALLRACES_HUMAN
     },
    {"elf", "elven", "elvenkind", "Elf",
     {0, 0},
     12, /* base speed, normal */
     PM_ELF, NON_PM, PM_ELF_MUMMY, PM_ELF_ZOMBIE,
     MRACE_ELF | ROLE_MALE | ROLE_FEMALE | ROLE_CHAOTIC,
     MRACE_ELF, MRACE_ELF, MRACE_ORC,
     /* Str Int Wis Dex Con Cha */
     {3, 3, 3, 3, 3, 3},
     {18, 20, 20, 18, 16, 18},
     /* Init Lower Higher */
     {1, 0, 0, 1, 1, 0},        /* Hit points */
     {2, 0, 3, 0, 3, 0},        /* Energy */
     UNLOCKFEAT_RACE_ELF, UNLOCKFEAT_RACE_ELF,
     },
    {"dwarf", "dwarven", "dwarvenkind", "Dwa",
     {0, 0},
     10, /* base speed, a bit slow */
     PM_DWARF, NON_PM, PM_DWARF_MUMMY, PM_DWARF_ZOMBIE,
     MRACE_DWARF | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL,
     MRACE_DWARF, MRACE_DWARF | MRACE_GNOME, MRACE_ORC,
     /* Str Int Wis Dex Con Cha */
     {3, 3, 3, 3, 3, 3},
     {STR18(100), 16, 16, 20, 20, 16},
     /* Init Lower Higher */
     {4, 0, 0, 3, 2, 0},        /* Hit points */
     {0, 0, 0, 0, 0, 0},        /* Energy */
     UNLOCKFEAT_RACE_DWARF, UNLOCKFEAT_RACE_DWARF,
     },
    {"gnome", "gnomish", "gnomehood", "Gno",
     {0, 0},
     11, /* base speed, just below normal */
     PM_GNOME, NON_PM, PM_GNOME_MUMMY, PM_GNOME_ZOMBIE,
     MRACE_GNOME | ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL,
     MRACE_GNOME, MRACE_DWARF | MRACE_GNOME, MRACE_HUMAN,
     /* Str Int Wis Dex Con Cha */
     {3, 3, 3, 3, 3, 3},
     {STR18(50), 19, 18, 18, 18, 18},
     /* Init Lower Higher */
     {1, 0, 0, 1, 0, 0},        /* Hit points */
     {2, 0, 2, 0, 2, 0},        /* Energy */
     UNLOCKFEAT_RACE_GNOME, UNLOCKFEAT_RACE_GNOME,
     },
    {"giant", "giant", "giantdom", "Gia",
     {"giant", "giantess"},
     6, /* base speed, slow */
     PM_GIANT, NON_PM, PM_GIANT_MUMMY, PM_GIANT_ZOMBIE,
     MRACE_GIANT | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_CHAOTIC,
     MRACE_GIANT, MRACE_GIANT, 0,
     /* Str Int Wis Dex Con Cha */
     {18, 3, 3, 3, 3, 3},
     {STR19(24), 15, 15, 18, 20, 16},
     /* Init Lower Higher */
     {1, 0, 0, 1, 0, 0},        /* Hit points */
     {1, 0, 1, 0, 1, 0},        /* Energy */
     UNLOCKFEAT_RACE_GIANT, UNLOCKFEAT_RACE_GIANT,
     },
    {"orc", "orcish", "orcdom", "Orc",
     {0, 0},
     12, /* base speed, normal for now (this may change) */
     PM_ORC, NON_PM, PM_ORC_MUMMY, PM_ORC_ZOMBIE,
     MRACE_ORC | ROLE_MALE | ROLE_FEMALE | ROLE_CHAOTIC,
     MRACE_ORC, 0, MRACE_HUMAN | MRACE_ELF | MRACE_DWARF,
     /* Str Int Wis Dex Con Cha */
     {3, 3, 3, 3, 3, 3},
     {STR18(50), 16, 16, 18, 18, 16},
     /* Init Lower Higher */
     {1, 0, 0, 1, 0, 0},        /* Hit points */
     {1, 0, 1, 0, 1, 0},        /* Energy */
     UNLOCKFEAT_RACE_ORC, UNLOCKFEAT_RACE_ORC,
     },
    {"sylph", "sylph", "sylphood", "Syl",
     {0,0},
     12, /* base speed, normal for now (this may change) */
     PM_SYLPH, NON_PM, NON_PM, NON_PM,
     MRACE_FAIRY | ROLE_FEMALE | ROLE_NEUTRAL | ROLE_CHAOTIC,
     MRACE_FAIRY, 0, 0,
     /* Str Int Wis Dex Con Cha */
     {3, 3, 3, 3, 3, 3},
     {16, 18, 19, 19, 17, 20},
     {2, 0, 1, 1, 1, 1},        /* Hit points */
     {2, 0, 3, 0, 2, 2},        /* Energy */
     UNLOCKFEAT_RACE_SYLPH, UNLOCKFEAT_RACE_SYLPH,
    },
    {"scurrier", "scurrid", "scurridae", "Scu",
     /* Yes, I am aware that "scurrier" and "sciurid" are etymologically
        unrelated, and that "sciurid" as in squirrel is spelled "sciurid".
        Scurriers, however, aren't squirrels as such; they're a heavily
        anthropomorphized cross between squirrel, rabbit (hence the digging
        ability), and all-round general-purpose (humanoid) rodent. */
     {"scurrier", "scurrier"},
     16, /* base speed on the fast side (may need tweaked) */
     PM_SCURRIER, NON_PM, NON_PM, NON_PM,
     MRACE_SCURRIER | ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL,
     MRACE_SCURRIER, MRACE_RODENT, MRACE_ORC,
     /* Str Int Wis Dex Con Cha */
     {2, 2, 3, 4, 2, 3},
     {16, 16, 18, 22, 15, 22},
     /* Init Lower Higher */
     {1, 0, 0, 1, 1, 0},        /* Hit points */
     {1, 0, 2, 0, 2, 0},        /* Energy */
     UNLOCKFEAT_RACE_SCURRIER, UNLOCKFEAT_RACE_SCURRIER,
     },
    {"valkyrie", "valkyrie", "Valhalla", "Val",
     {"valkyrie", "valkyrie"},
     12, /* base speed */
     PM_VALKYRIE, PM_VALKYRIE, NON_PM, NON_PM,
     MRACE_VALKYRIE | ROLE_FEMALE | ROLE_NEUTRAL,
     MRACE_VALKYRIE, MRACE_VALKYRIE, MRACE_DWARF | MRACE_GIANT,
     /* Str Int Wis Dex Con Cha */
     {3, 2, 4, 2, 4, 3},
     {STR19(20), 17, 19, 17, 18, 18},
     /* Init Lower Higher */
     {2, 0, 1, 0, 1, 0},        /* Hit points */
     {1, 0, 3, 0, 3, 0},        /* Energy */
     UNLOCKFEAT_RACE_VALKYRIE, UNLOCKFEAT_RACE_VALKYRIE
    },
/* Array terminator */
    {0, 0, 0, 0}
};


/* The player's race, created at runtime from initial
 * choices.  This will be munged in role_init().
 */
struct Race urace;


/* Table of all genders */
const struct Gender genders[] = {
    {"male", "he", "him", "his", "Mal", ROLE_MALE},
    {"female", "she", "her", "her", "Fem", ROLE_FEMALE},
    {"neuter", "it", "it", "its", "Ntr", ROLE_NEUTER}
};


/* Table of all alignments */
const struct Align aligns[] = {
    {"law", "lawful", "Law", ROLE_LAWFUL, A_LAWFUL},
    {"balance", "neutral", "Neu", ROLE_NEUTRAL, A_NEUTRAL},
    {"chaos", "chaotic", "Cha", ROLE_CHAOTIC, A_CHAOTIC},
    {"evil", "unaligned", "Una", 0, A_NONE}
};

static char *promptsep(char *, int);
static int role_gendercount(int);
static int race_alignmentcount(int);

/* used by nh_str2XXX() */
static const char randomstr[] = "random";

/* Can a monster feel the ground with its toes?
   Relevant for sylph players. */
boolean
can_feel_ground(struct monst *mon)
{
    schar floortype = level->locations[m_mx(mon)][m_my(mon)].typ;
    boolean lev = (mon == &youmonst) ? Levitation : FALSE;
    /* Can monsters levitate?  If, for example, they find lev boots? */
    return !which_armor(mon, os_armf) && !lev &&
        !(floortype == LAVAPOOL || floortype == ALTAR ||
          floortype == DRAWBRIDGE_DOWN || floortype == AIR ||
          floortype == THRONE || floortype == ICE);
}

static
boolean
only_sylph_safe_armor(struct monst *mon, enum objslot slot)
{
    struct obj *armor = which_armor(mon, slot);
    return !armor || objects[armor->otyp].oc_material == WOOD ||
                     objects[armor->otyp].oc_material == CLOTH ||
                     armor->oartifact == urole.questarti;
    /* Leather is not ok mostly for gameplay reasons (allowing leather would
       allow a lot of armor), but there's also a flavor justification:  leather
       is too worked/unnatural and doesn't "breathe" at all.  (Wood doesn't
       really breathe either, but the only wooden armors are shields, which are
       more held than worn and so don't really cover the skin.) */
}

boolean
can_draw_from_environment(struct monst *mon, boolean healing)
{
    enum objslot s;
    if (monsndx(mon->data) == PM_SYLPH ||
        ((mon == &youmonst) && Race_if(PM_SYLPH) && !Upolyd)) {
        /* Can you feel the air on your skin? */
        for (s = 0; s <= os_last_armor; s++)
            if (!only_sylph_safe_armor(mon, s))
                return FALSE;
        return (/* can your toes feel the ground? (water is ok) */
            can_feel_ground(mon) &&
            /* Can't draw healing in Gehennom most of the time */
            (!Inhell || !healing || !(moves % 7)) &&
            (u.uhs < WEAK));
    }
    return FALSE;
}

boolean
validrole(int rolenum)
{
    return rolenum >= 0 && rolenum < SIZE(roles) - 1;
}


int
randrole(enum rng rng)
{
    return rn2_on_rng(SIZE(roles) - 1, rng);
}


int
str2role(char *str)
{
    int i, len;

    /* Is str valid? */
    if (!str || !str[0])
        return ROLE_NONE;

    /* Match as much of str as is provided */
    len = strlen(str);
    for (i = 0; roles[i].name.m; i++) {
        /* Does it match the male name? */
        if (!strncmpi(str, roles[i].name.m, len))
            return i;
        /* Or the female name? */
        if (roles[i].name.f && !strncmpi(str, roles[i].name.f, len))
            return i;
        /* Or the filecode? */
        if (!strcmpi(str, roles[i].filecode))
            return i;
    }

    if ((len == 1 && (*str == '*' || *str == '@')) ||
        !strncmpi(str, randomstr, len))
        return ROLE_RANDOM;

    /* Couldn't find anything appropriate */
    return ROLE_NONE;
}


boolean
validrace(int rolenum, int racenum)
{
    /* Assumes nh_validrole */
    return (racenum >= 0 && racenum < SIZE(races) - 1 &&
            (roles[rolenum].allow & races[racenum].allow & ROLE_RACEMASK));
}


int
str2race(char *str)
{
    int i, len;

    /* Is str valid? */
    if (!str || !str[0])
        return ROLE_NONE;

    /* Match as much of str as is provided */
    len = strlen(str);
    for (i = 0; races[i].noun; i++) {
        /* Does it match the noun? */
        if (!strncmpi(str, races[i].noun, len))
            return i;
        /* Or the filecode? */
        if (!strcmpi(str, races[i].filecode))
            return i;
    }

    if ((len == 1 && (*str == '*' || *str == '@')) ||
        !strncmpi(str, randomstr, len))
        return ROLE_RANDOM;

    /* Couldn't find anything appropriate */
    return ROLE_NONE;
}


boolean
validgend(int rolenum, int racenum, int gendnum)
{
    /* Assumes nh_validrole and nh_validrace */
    return (gendnum >= 0 && gendnum < ROLE_GENDERS &&
            (roles[rolenum].allow & races[racenum].allow &
             genders[gendnum].allow & ROLE_GENDMASK));
}


int
str2gend(char *str)
{
    int i, len;

    /* Is str valid? */
    if (!str || !str[0])
        return ROLE_NONE;

    /* Match as much of str as is provided */
    len = strlen(str);
    for (i = 0; i < ROLE_GENDERS; i++) {
        /* Does it match the adjective? */
        if (!strncmpi(str, genders[i].adj, len))
            return i;
        /* Or the filecode? */
        if (!strcmpi(str, genders[i].filecode))
            return i;
    }
    if ((len == 1 && (*str == '*' || *str == '@')) ||
        !strncmpi(str, randomstr, len))
        return ROLE_RANDOM;

    /* Couldn't find anything appropriate */
    return ROLE_NONE;
}


boolean
validalign(int rolenum, int racenum, int alignnum)
{
    /* Assumes nh_validrole and nh_validrace */
    return (alignnum >= 0 && alignnum < ROLE_ALIGNS &&
            (roles[rolenum].allow & races[racenum].allow &
             aligns[alignnum].allow & ROLE_ALIGNMASK));
}


int
str2align(char *str)
{
    int i, len;

    /* Is str valid? */
    if (!str || !str[0])
        return ROLE_NONE;

    /* Match as much of str as is provided */
    len = strlen(str);
    for (i = 0; i < ROLE_ALIGNS; i++) {
        /* Does it match the adjective? */
        if (!strncmpi(str, aligns[i].adj, len))
            return i;
        /* Or the filecode? */
        if (!strcmpi(str, aligns[i].filecode))
            return i;
    }
    if ((len == 1 && (*str == '*' || *str == '@')) ||
        !strncmpi(str, randomstr, len))
        return ROLE_RANDOM;

    /* Couldn't find anything appropriate */
    return ROLE_NONE;
}


/* is racenum compatible with any rolenum/gendnum/alignnum constraints? */
boolean
ok_race(int rolenum, int racenum, int gendnum, int alignnum)
{
    int i;
    short allow;

    if (racenum >= 0 && racenum < SIZE(races) - 1) {
        allow = races[racenum].allow;
        if (rolenum >= 0 && rolenum < SIZE(roles) - 1 &&
            !(allow & roles[rolenum].allow & ROLE_RACEMASK))
            return FALSE;
        if (gendnum >= 0 && gendnum < ROLE_GENDERS &&
            !(allow & genders[gendnum].allow & ROLE_GENDMASK))
            return FALSE;
        if (alignnum >= 0 && alignnum < ROLE_ALIGNS &&
            !(allow & aligns[alignnum].allow & ROLE_ALIGNMASK))
            return FALSE;
        return TRUE;
    } else {
        for (i = 0; i < SIZE(races) - 1; i++) {
            allow = races[i].allow;
            if (rolenum >= 0 && rolenum < SIZE(roles) - 1 &&
                !(allow & roles[rolenum].allow & ROLE_RACEMASK))
                continue;
            if (gendnum >= 0 && gendnum < ROLE_GENDERS &&
                !(allow & genders[gendnum].allow & ROLE_GENDMASK))
                continue;
            if (alignnum >= 0 && alignnum < ROLE_ALIGNS &&
                !(allow & aligns[alignnum].allow & ROLE_ALIGNMASK))
                continue;
            return TRUE;
        }
        return FALSE;
    }
}


/* is gendnum compatible with any rolenum/racenum/alignnum constraints? */
/* gender and alignment are not comparable (and also not constrainable) */
boolean
ok_gend(int rolenum, int racenum, int gendnum, int alignnum)
{
    int i;
    short allow;

    if (gendnum >= 0 && gendnum < ROLE_GENDERS) {
        allow = genders[gendnum].allow;
        if (rolenum >= 0 && rolenum < SIZE(roles) - 1 &&
            !(allow & roles[rolenum].allow & ROLE_GENDMASK))
            return FALSE;
        if (racenum >= 0 && racenum < SIZE(races) - 1 &&
            !(allow & races[racenum].allow & ROLE_GENDMASK))
            return FALSE;
        return TRUE;
    } else {
        for (i = 0; i < ROLE_GENDERS; i++) {
            allow = genders[i].allow;
            if (rolenum >= 0 && rolenum < SIZE(roles) - 1 &&
                !(allow & roles[rolenum].allow & ROLE_GENDMASK))
                continue;
            if (racenum >= 0 && racenum < SIZE(races) - 1 &&
                !(allow & races[racenum].allow & ROLE_GENDMASK))
                continue;
            return TRUE;
        }
        return FALSE;
    }
}


/* is alignnum compatible with any rolenum/racenum/gendnum constraints? */
/* alignment and gender are not comparable (and also not constrainable) */
boolean
ok_align(int rolenum, int racenum, int gendnum, int alignnum)
{
    int i;
    short allow;

    // TODO:  XXX YOU ARE HERE
/*
    if (rolenum >= 0 && rolenum < SIZE(roles) - 1 &&
        !is_unlocked_feature(UNLOCK_FIELD_ROLE,
                             roles[rolenum].unlocked)) {
        return FALSE;
    }
    if (racenum >= 0 && racenum < SIZE(races) - 1 &&
        !is_unlocked_feature(UNLOCK_FIELD_RACE,
                             races[racenum].unlockedrace)) {
        return FALSE;
    }
*/
    if (alignnum >= 0 && alignnum < ROLE_ALIGNS) {
        allow = aligns[alignnum].allow;
        if (rolenum >= 0 && rolenum < SIZE(roles) - 1 &&
            !(allow & roles[rolenum].allow & ROLE_ALIGNMASK))
            return FALSE;
        if (racenum >= 0 && racenum < SIZE(races) - 1 &&
            !(allow & races[racenum].allow & ROLE_ALIGNMASK))
            return FALSE;
        return TRUE;
    } else {
        for (i = 0; i < ROLE_ALIGNS; i++) {
            allow = races[i].allow;
            if (rolenum >= 0 && rolenum < SIZE(roles) - 1 &&
                !(allow & roles[rolenum].allow & ROLE_ALIGNMASK))
                continue;
            if (racenum >= 0 && racenum < SIZE(races) - 1 &&
                !(allow & races[racenum].allow & ROLE_ALIGNMASK))
                continue;
            return TRUE;
        }
        return FALSE;
    }
}


struct nh_roles_info *
nh_get_roles(void)
{
    int i, rolenum, racenum, gendnum, alignnum, arrsize;
    struct nh_roles_info *info;
    const char **names, **names2;
    nh_bool *tmpmatrix;

    xmalloc_cleanup(&api_blocklist);

    info = xmalloc(&api_blocklist, sizeof (struct nh_roles_info));

    /* number of choices */
    for (i = 0; roles[i].name.m; i++)
        ;
    info->num_roles = i;

    for (i = 0; races[i].noun; i++)
        ;
    info->num_races = i;

    info->num_genders = ROLE_GENDERS;
    info->num_aligns = ROLE_ALIGNS;

    /* names of choices */
    names = xmalloc(&api_blocklist, info->num_roles * sizeof (char *));
    names2 = xmalloc(&api_blocklist, info->num_roles * sizeof (char *));
    for (i = 0; i < info->num_roles; i++) {
        names[i] = roles[i].name.m;
        names2[i] = roles[i].name.f;
    }
    info->rolenames_m = names;
    info->rolenames_f = names2;

    names = xmalloc(&api_blocklist, info->num_races * sizeof (char *));
    for (i = 0; i < info->num_races; i++)
        names[i] = races[i].noun;
    info->racenames = names;

    names = xmalloc(&api_blocklist, info->num_genders * sizeof (char *));
    for (i = 0; i < info->num_genders; i++)
        names[i] = genders[i].adj;
    info->gendnames = names;

    names = xmalloc(&api_blocklist, info->num_aligns * sizeof (char *));
    for (i = 0; i < info->num_aligns; i++)
        names[i] = aligns[i].adj;
    info->alignnames = names;

    /* valid combinations of choices */
    arrsize =
        info->num_roles * info->num_races * info->num_genders *
        info->num_aligns;
    tmpmatrix = xmalloc(&api_blocklist, arrsize * sizeof (nh_bool));
    memset(tmpmatrix, FALSE, arrsize * sizeof (nh_bool));
    for (rolenum = 0; rolenum < info->num_roles; rolenum++) {
        for (racenum = 0; racenum < info->num_races; racenum++) {
            if (!ok_race(rolenum, racenum, ROLE_NONE, ROLE_NONE))
                continue;
            for (gendnum = 0; gendnum < info->num_genders; gendnum++) {
                if (!ok_gend(rolenum, racenum, gendnum, ROLE_NONE))
                    continue;
                for (alignnum = 0; alignnum < info->num_aligns; alignnum++) {
                    tmpmatrix[nh_cm_idx
                              ((*info), rolenum, racenum, gendnum, alignnum)] =
                        ok_align(rolenum, racenum, gendnum, alignnum);
                }
            }
        }
    }
    info->matrix = tmpmatrix;

    return info;
}


#define BP_ALIGN        0
#define BP_GEND         1
#define BP_RACE         2
#define BP_ROLE         3
#define NUM_BP          4

static char pa[NUM_BP], post_attribs;

static char *
promptsep(char *buf, int num_post_attribs)
{
    const char *conj = "and ";

    if (num_post_attribs > 1 && post_attribs < num_post_attribs &&
        post_attribs > 1)
        strcat(buf, ",");
    strcat(buf, " ");
    --post_attribs;
    if (!post_attribs && num_post_attribs > 1)
        strcat(buf, conj);
    return buf;
}

static int
role_gendercount(int rolenum)
{
    int gendcount = 0;

    if (validrole(rolenum)) {
        if (roles[rolenum].allow & ROLE_MALE)
            ++gendcount;
        if (roles[rolenum].allow & ROLE_FEMALE)
            ++gendcount;
        if (roles[rolenum].allow & ROLE_NEUTER)
            ++gendcount;
    }
    return gendcount;
}

static int
race_alignmentcount(int racenum)
{
    int aligncount = 0;

    if (racenum != ROLE_NONE && racenum != ROLE_RANDOM) {
        if (races[racenum].allow & ROLE_CHAOTIC)
            ++aligncount;
        if (races[racenum].allow & ROLE_LAWFUL)
            ++aligncount;
        if (races[racenum].allow & ROLE_NEUTRAL)
            ++aligncount;
    }
    return aligncount;
}


/* This uses a hardcoded BUFSZ, not the msg* functions, because it runs
   outside the main game sequence. */
const char *
nh_root_plselection_prompt(char *suppliedbuf, int buflen, int rolenum,
                           int racenum, int gendnum, int alignnum)
{
    int k, gendercount = 0, aligncount = 0;
    char buf[BUFSZ];
    static const char err_ret[] = " character's";
    boolean donefirst = FALSE;

    xmalloc_cleanup(&api_blocklist);

    if (!suppliedbuf || buflen < 1)
        return err_ret;

    /* initialize these static variables each time this is called */
    post_attribs = 0;
    for (k = 0; k < NUM_BP; ++k)
        pa[k] = 0;
    buf[0] = '\0';
    *suppliedbuf = '\0';

    /* How many alignments are allowed for the desired race? */
    if (racenum != ROLE_NONE && racenum != ROLE_RANDOM)
        aligncount = race_alignmentcount(racenum);

    if (alignnum != ROLE_NONE && alignnum != ROLE_RANDOM) {
        /* if race specified, and multiple choice of alignments for it */
        if (donefirst)
            strcat(buf, " ");
        strcat(buf, aligns[alignnum].adj);
        donefirst = TRUE;
    } else {
        /* if alignment not specified, but race is specified and only one
           choice of alignment for that race then don't include it in the later 
           list */
        if ((((racenum != ROLE_NONE && racenum != ROLE_RANDOM) &&
              ok_race(rolenum, racenum, gendnum, alignnum))
             && (aligncount > 1))
            || (racenum == ROLE_NONE || racenum == ROLE_RANDOM)) {
            pa[BP_ALIGN] = 1;
            post_attribs++;
        }
    }
    /* <your lawful> */

    /* How many genders are allowed for the desired role? */
    if (validrole(rolenum))
        gendercount = role_gendercount(rolenum);

    if (gendnum != ROLE_NONE && gendnum != ROLE_RANDOM) {
        if (validrole(rolenum)) {
            /* if role specified, and multiple choice of genders for it, and
               name of role itself does not distinguish gender */
            if ((rolenum != ROLE_NONE) && (gendercount > 1)
                && !roles[rolenum].name.f) {
                if (donefirst)
                    strcat(buf, " ");
                strcat(buf, genders[gendnum].adj);
                donefirst = TRUE;
            }
        } else {
            if (donefirst)
                strcat(buf, " ");
            strcat(buf, genders[gendnum].adj);
            donefirst = TRUE;
        }
    } else {
        /* if gender not specified, but role is specified and only one choice
           of gender then don't include it in the later list */
        if ((validrole(rolenum) && (gendercount > 1)) || !validrole(rolenum)) {
            pa[BP_GEND] = 1;
            post_attribs++;
        }
    }
    /* <your lawful female> */

    if (racenum != ROLE_NONE && racenum != ROLE_RANDOM) {
        if (validrole(rolenum) &&
            ok_race(rolenum, racenum, gendnum, alignnum)) {
            if (donefirst)
                strcat(buf, " ");
            strcat(buf,
                   (rolenum ==
                    ROLE_NONE) ? races[racenum].noun : races[racenum].adj);
            donefirst = TRUE;
        } else if (!validrole(rolenum)) {
            if (donefirst)
                strcat(buf, " ");
            strcat(buf, races[racenum].noun);
            donefirst = TRUE;
        } else {
            pa[BP_RACE] = 1;
            post_attribs++;
        }
    } else {
        pa[BP_RACE] = 1;
        post_attribs++;
    }
    /* <your lawful female gnomish> || <your lawful female gnome> */

    if (validrole(rolenum)) {
        if (donefirst)
            strcat(buf, " ");
        if (gendnum != ROLE_NONE) {
            if (gendnum == 1 && roles[rolenum].name.f)
                strcat(buf, roles[rolenum].name.f);
            else
                strcat(buf, roles[rolenum].name.m);
        } else {
            if (roles[rolenum].name.f) {
                strcat(buf, roles[rolenum].name.m);
                strcat(buf, "/");
                strcat(buf, roles[rolenum].name.f);
            } else
                strcat(buf, roles[rolenum].name.m);
        }
        donefirst = TRUE;
    } else if (rolenum == ROLE_NONE) {
        pa[BP_ROLE] = 1;
        post_attribs++;
    }

    if ((racenum == ROLE_NONE || racenum == ROLE_RANDOM) &&
        !validrole(rolenum)) {
        if (donefirst)
            strcat(buf, " ");
        strcat(buf, "character");
    }
    /* <your lawful female gnomish cavewoman> || <your lawful female gnome> ||
       <your lawful female character> */
    if (buflen > (int)(strlen(buf) + 1)) {
        strcpy(suppliedbuf, buf);
        return suppliedbuf;
    } else
        return err_ret;
}

/* This uses a hardcoded BUFSZ, not the msg* functions, because it runs
   outside the main game sequence. */
char *
nh_build_plselection_prompt(char *buf, int buflen, int rolenum, int racenum,
                            int gendnum, int alignnum)
{
    const char *defprompt = "Shall I pick a character for you?";
    int num_post_attribs = 0;
    char tmpbuf[BUFSZ];

    xmalloc_cleanup(&api_blocklist);

    if (buflen < QBUFSZ) {
        strncpy(buf, defprompt, buflen);
        buf[buflen-1] = '\0'; /* strncpy doesn't \0-terminate on overflow */
        return buf;
    }

    strcpy(tmpbuf, "Shall I pick ");
    if (racenum != ROLE_NONE || validrole(rolenum))
        strcat(tmpbuf, "your ");
    else {
        strcat(tmpbuf, "a ");
    }
    /* <your> */

    nh_root_plselection_prompt(
        tmpbuf + strlen(tmpbuf), buflen - strlen(tmpbuf),
        rolenum, racenum, gendnum, alignnum);
    /* A manual 's is used here because s_suffix will allocate onto the
     * turnstate chain, which leads to a leak. Furthermore, all races are
     * singular, so this is more grammatically correct.
     */
    sprintf(buf, "%s's", tmpbuf);

    /* buf should now be: < your lawful female gnomish cavewoman's> || <your
       lawful female gnome's> || <your lawful female character's> Now append
       the post attributes to it */

    num_post_attribs = post_attribs;
    if (post_attribs) {
        if (pa[BP_RACE]) {
            promptsep(buf + strlen(buf), num_post_attribs);
            strcat(buf, "race");
        }
        if (pa[BP_ROLE]) {
            promptsep(buf + strlen(buf), num_post_attribs);
            strcat(buf, "role");
        }
        if (pa[BP_GEND]) {
            promptsep(buf + strlen(buf), num_post_attribs);
            strcat(buf, "gender");
        }
        if (pa[BP_ALIGN]) {
            promptsep(buf + strlen(buf), num_post_attribs);
            strcat(buf, "alignment");
        }
    }
    strcat(buf, " for you?");
    return buf;
}

#undef BP_ALIGN
#undef BP_GEND
#undef BP_RACE
#undef BP_ROLE
#undef NUM_BP


/*
 * Special setup modifications here:
 *
 * Unfortunately, this is going to have to be done
 * on each newgame or restore, because you lose the permonst mods
 * across a save/restore.  :-)
 *
 *      1 - The Rogue Leader is the Tourist Nemesis.
 *      2 - Priests start with a random alignment - convert the leader and
 *          guardians here.
 *      3 - Elves can have one of two different leaders, but can't work it
 *          out here because it requires hacking the level file data (see
 *          sp_lev.c).
 *
 * This code also replaces quest_init().
 */
void
role_init(void)
{
    int alignmnt;

    alignmnt = aligns[u.initalign].value;

    /* Initialize urole and urace */
    urole = roles[u.initrole];
    urace = races[u.initrace];

    /* Fix up the quest leader */
    pm_leader = mons[urole.ldrnum];
    if (urole.ldrnum != NON_PM) {
        pm_leader.msound = MS_LEADER;
        pm_leader.mflags2 |= (M2_PEACEFUL);
        pm_leader.mflags3 |= M3_CLOSE;
        pm_leader.maligntyp = alignmnt * 3;
    }

    /* Fix up the quest guardians */
    pm_guardian = mons[urole.guardnum];
    if (urole.guardnum != NON_PM) {
        pm_guardian.mflags2 |= (M2_PEACEFUL);
        pm_guardian.maligntyp = alignmnt * 3;
    }

    /* Fix up the quest nemesis */
    pm_nemesis = mons[urole.neminum];
    if (urole.neminum != NON_PM) {
        pm_nemesis.msound = MS_NEMESIS;
        pm_nemesis.mflags2 &= ~(M2_PEACEFUL);
        pm_nemesis.mflags2 |= (M2_NASTY | M2_STALK | M2_HOSTILE);
        pm_nemesis.mflags3 |= M3_WANTSARTI | M3_WAITFORU;
    }

    /* Fix up infravision */
    pm_you_male = pm_you_female = mons[urole.malenum];
    if (urole.femalenum != NON_PM)
        pm_you_female = mons[urole.femalenum];
    if (mons[urace.malenum].mflags3 & M3_INFRAVISION) {
        /* although an infravision intrinsic is possible, infravision is purely 
           a property of the physical race.  This means that we must put the
           infravision flag in the player's current race (either that or have
           separate permonst entries for elven/non-elven members of each
           class).  The side effect is that all NPCs of that class will have
           (probably bogus) infravision, but since infravision has no effect
           for NPCs anyway we can ignore this. */
        pm_you_male.mflags3 |= M3_INFRAVISION;
        pm_you_female.mflags3 |= M3_INFRAVISION;
    }

    /* Artifacts are fixed in hack_artifacts() */

    /* Success! */
    return;
}

/* Initialize the player's pantheon.
 *
 * This must come after both u and urole are initialized by role_init 
 * and by either u_init or restore_you
 */
void
pantheon_init(boolean newgame)
{
    if (newgame) {
        u.upantheon = u.initrole;            /* use own gods */
        while (!roles[u.upantheon].lgod)     /* unless they're missing */
            u.upantheon = randrole(rng_charstats_role);
    }

    if (!urole.lgod) {
        urole.lgod = roles[u.upantheon].lgod;
        urole.ngod = roles[u.upantheon].ngod;
        urole.cgod = roles[u.upantheon].cgod;
    }
}

const char *
Hello(struct monst *mtmp)
{
    switch (Role_switch) {
    case PM_KNIGHT:
        return "Salutations";   /* Olde English */
    case PM_SAMURAI:
        return (mtmp && mtmp->data == &mons[PM_SHOPKEEPER] ?
                "Irasshaimase" : "Konnichi wa");   /* Japanese */
    case PM_TOURIST:
        return "Aloha"; /* Hawaiian */
    case PM_SHIELDMAIDEN:
    case PM_HOPLITE:
        return "Velkommen";     /* Norse */
    default:
        return "Hello";
    }
}

const char *
Goodbye(void)
{
    switch (Role_switch) {
    case PM_KNIGHT:
        return "Fare thee well";        /* Olde English */
    case PM_SAMURAI:
        return "Sayonara";      /* Japanese */
    case PM_TOURIST:
        return "Aloha"; /* Hawaiian */
    case PM_VALKYRIE:
        return "Farvel";        /* Norse */
    default:
        return "Goodbye";
    }
}

/* role.c */

