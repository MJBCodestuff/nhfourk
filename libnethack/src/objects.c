/* vim:set cin ft=c sw=4 sts=4 ts=8 et ai cino=Ls\:0t0(0 : -*- mode:c;fill-column:80;tab-width:8;c-basic-offset:4;indent-tabs-mode:nil;c-file-style:"k&r" -*-*/
/* Last modified by Alex Smith, 2015-11-11 */
/* Copyright (c) Mike Threepoint, 1989.                           */
/* NetHack may be freely redistributed.  See license for details. */

#ifndef OBJECTS_PASS_2_
/* first pass */
struct monst {
    struct monst *dummy;
};      /* lint: struct obj's union */

# include "config.h"
# include "obj.h"
# include "objclass.h"
# include "prop.h"
# include "skills.h"
# include "monflag.h"

#else /* !OBJECTS_PASS_2_ */
/* second pass */
# include "color.h"
# define COLOR_FIELD(X) X,
#endif /* !OBJECTS_PASS_2_ */


/* objects have symbols: ) [ = " ( % ! ? + / $ * ` 0 _ . */

/*
 * Note:  OBJ() and BITS() macros are used to avoid exceeding argument
 * limits imposed by some compilers.  The ctnr field of BITS currently
 * does not map into struct objclass, and is ignored in the expansion.
 * The 0 in the expansion corresponds to oc_pre_discovered, which is
 * set at run-time during role-specific character initialization.
 */

#ifndef OBJECTS_PASS_2_
/* first pass -- object descriptive text */
# define OBJ(name,desc) name,desc
# define OBJECT(obj,bits,prp,sym,prob,dly,wt,cost,sdam,ldam,oc1,oc2,nut,color) \
             {obj}
# define OBJECT2(obj,bits,p1,p2,sym,prob,dly,wt,cost,sd,ld,oc1,oc2,nut,color) \
             {obj}

const struct objdescr obj_descr[] = {
#else
/* second pass -- object definitions */

# define BITS(nmkn,mrg,uskn,ctnr,mgc,chrg,uniq,nwsh,big,tuf,dir,sub,mtrl) \
        nmkn,mrg,uskn,0,mgc,chrg,uniq,nwsh,0,big,tuf,dir,mtrl,sub
# define OBJECT(obj,bits,prp,sym,prob,dly,wt,cost,sdam,ldam,oc1,oc2,nut,color) \
        {0, 0, NULL, bits, prp, 0, sym, dly, COLOR_FIELD(color) \
         prob, wt, cost, sdam, ldam, oc1, oc2, nut}
/* OBJECT2 differs from OBJECT by having the second property field.
   Currently, it is only used by certain kinds of armor. */
# define OBJECT2(obj,bits,prp,pr2,sym,prob,dly,wt,cost,sd,ld,oc1,oc2,nut,clr) \
        {0, 0, NULL, bits, prp, pr2, sym, dly, COLOR_FIELD(clr) \
         prob, wt, cost, sd, ld, oc1, oc2, nut}
# define HARDGEM(n) (n >= 8)

struct objclass *objects;

const struct objclass const_objects[] = {
#endif
/* dummy object[0] -- description [2nd arg] *must* be NULL */
    OBJECT(OBJ("strange object", NULL),
           BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, 0),
           0, ILLOBJ_CLASS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),

/* weapons ... */
#define WEAPON(name,app,kn,mg,bi,prob,wt,cost,sdam,ldam,hitbon,typ,sub,metal, \
               color) \
        OBJECT( \
            OBJ(name,app), BITS(kn,mg,1,0,0,1,0,0,bi,0,typ,sub,metal), 0, \
            WEAPON_CLASS, prob, 0, \
            wt, cost, sdam, ldam, hitbon, 0, wt, color )
#define PROJECTILE(name,app,kn,prob,wt,cost,sdam,ldam,hitbon,metal,sub,color) \
        OBJECT( \
           OBJ(name,app), \
           BITS(kn,1,1,0,0,1,0,0,0,0,PIERCE,sub,metal), 0, \
           WEAPON_CLASS, prob, 0, \
           wt, cost, sdam, ldam, hitbon, 0, wt, color )
#define BOW(name,app,kn,prob,wt,cost,hitbon,metal,sub,color) \
        OBJECT( \
           OBJ(name,app), BITS(kn,0,1,0,0,1,0,0,0,0,0,sub,metal), 0, \
           WEAPON_CLASS, prob, 0, \
           wt, cost, 2, 2, hitbon, 0, wt, color )

/* Note: for weapons that don't do an even die of damage (ex. 2-7 or 3-18)
 * the extra damage is added on in weapon.c, not here! */

#define P PIERCE
#define S SLASH
#define B WHACK

/* missiles */
    PROJECTILE("arrow", NULL,
               1, 55, 1, 2, 7, 6, 0, IRON, -P_BOW, HI_METAL),
    PROJECTILE("elven arrow", "runed arrow",
               0, 20, 1, 2, 8, 6, 0, WOOD, -P_BOW, HI_WOOD),
    PROJECTILE("orcish arrow", "crude arrow",
               0, 20, 1, 2, 6, 6, 0, IRON, -P_BOW, CLR_BLACK),
    PROJECTILE("silver arrow", NULL,
               1, 12, 1, 5, 7, 6, 0, SILVER, -P_BOW, HI_SILVER),
    PROJECTILE("ya", "bamboo arrow",
               0, 15, 1, 4, 8, 7, 1, METAL, -P_BOW, HI_METAL),
    PROJECTILE("crossbow bolt", NULL,
               1, 55, 1, 2, 5, 6, 0, IRON, -P_CROSSBOW, HI_METAL),

    WEAPON("dart", NULL,
           1, 1, 0, 60, 1, 2, 4, 3, 0, P, -P_DART, IRON, HI_METAL),
    WEAPON("shuriken", "throwing star",
           0, 1, 0, 35, 1, 5, 8, 6, 2, P, -P_SHURIKEN, IRON, HI_METAL),
    WEAPON("boomerang", NULL,
           1, 1, 0, 15, 5, 20, 9, 9, 0, 0, -P_BOOMERANG, WOOD, HI_WOOD),

/* spears */
    WEAPON("spear", NULL,
           1, 1, 0, 50, 30, 3, 6, 8, 0, P, P_SPEAR, IRON, HI_METAL),
    WEAPON("elven spear", "runed spear",
           0, 1, 0, 10, 30, 3, 7, 8, 0, P, P_SPEAR, WOOD, HI_WOOD),
    WEAPON("orcish spear", "crude spear",
           0, 1, 0, 13, 30, 3, 5, 8, 0, P, P_SPEAR, IRON, CLR_BLACK),
    WEAPON("dwarvish spear", "stout spear",
           0, 1, 0, 12, 35, 3, 8, 8, 0, P, P_SPEAR, IRON, HI_METAL),
    WEAPON("silver spear", NULL,
           1, 1, 0, 2, 36, 40, 6, 8, 0, P, P_SPEAR, SILVER, HI_SILVER),
    WEAPON("javelin", "throwing spear",
           0, 1, 0, 10, 20, 3, 6, 6, 0, P, P_JAVELIN, IRON, HI_METAL),

    WEAPON("trident", NULL,
           1, 0, 0, 8, 25, 5, 6, 4, 0, P, P_TRIDENT, IRON, HI_METAL),
    /* +1 small, +2d4 large */

/* blades */
    WEAPON("dagger", NULL,
           1, 1, 0, 30, 10, 4, 4, 3, 2, P, P_DAGGER, IRON, HI_METAL),
    WEAPON("elven dagger", "runed dagger",
           0, 1, 0, 10, 10, 4, 5, 3, 2, P, P_DAGGER, WOOD, HI_WOOD),
    WEAPON("orcish dagger", "crude dagger",
           0, 1, 0, 12, 10, 4, 3, 3, 2, P, P_DAGGER, IRON, CLR_BLACK),
    WEAPON("silver dagger", NULL,
           1, 1, 0, 3, 12, 40, 4, 3, 2, P, P_DAGGER, SILVER, HI_SILVER),
    WEAPON("athame", NULL,
           1, 1, 0, 0, 10, 4, 4, 3, 2, S, P_DAGGER, IRON, HI_METAL),
    WEAPON("scalpel", NULL,
           1, 1, 0, 0, 5, 6, 3, 3, 2, S, P_KNIFE, METAL, HI_METAL),
    WEAPON("knife", NULL,
           1, 1, 0, 25, 5, 4, 3, 2, 0, P | S, P_KNIFE, IRON, HI_METAL),
    WEAPON("worm tooth", NULL,
           1, 0, 0, 0, 20, 2, 2, 2, 0, 0, P_KNIFE, 0, CLR_WHITE),
    WEAPON("crysknife", NULL,
           1, 0, 0, 0, 20, 100, 10, 10, 3, P, P_KNIFE, MINERAL, CLR_WHITE),

    WEAPON("axe", NULL,
           1, 0, 0, 40, 60, 8, 6, 4, 0, S, P_AXE, IRON, HI_METAL),
    WEAPON("battle-axe", "double-headed axe",
           0, 0, 1, 10, 120, 40, 8, 6, 0, S, P_AXE, IRON, HI_METAL),
    /* "double-bitted" ? */

/* swords */
    WEAPON("short sword", NULL,
           1, 0, 0, 8, 30, 10, 6, 8, 0, P, P_SHORT_SWORD, IRON, HI_METAL),
    WEAPON("elven short sword", "runed short sword",
           0, 0, 0, 2, 30, 10, 8, 8, 0, P, P_SHORT_SWORD, WOOD, HI_WOOD),
    WEAPON("orcish short sword", "crude short sword",
           0, 0, 0, 3, 30, 10, 5, 8, 0, P, P_SHORT_SWORD, IRON, CLR_BLACK),
    WEAPON("dwarvish short sword", "broad short sword",
           0, 0, 0, 2, 30, 10, 7, 8, 0, P, P_SHORT_SWORD, IRON, HI_METAL),
    WEAPON("scimitar", "curved sword",
           0, 0, 0, 15, 40, 15, 8, 8, 0, S, P_SCIMITAR, IRON, HI_METAL),
    WEAPON("silver saber", NULL,
           1, 0, 0, 6, 40, 75, 8, 8, 0, S, P_SABER, SILVER, HI_SILVER),
    WEAPON("broadsword", NULL,
           1, 0, 0, 8, 70, 10, 4, 6, 0, S, P_BROAD_SWORD, IRON, HI_METAL),
    /* +d4 small, +1 large */
    WEAPON("elven broadsword", "runed broadsword",
           0, 0, 0, 4, 70, 10, 6, 6, 0, S, P_BROAD_SWORD, WOOD, HI_WOOD),
    /* +d4 small, +1 large */
    WEAPON("long sword", NULL,
           1, 0, 0, 50, 40, 15, 8, 12, 0, S, P_LONG_SWORD, IRON, HI_METAL),
    WEAPON("two-handed sword", NULL,
           1, 0, 1, 22, 150, 50, 12, 6, 0, S, P_TWO_HANDED_SWORD, IRON,
           HI_METAL),
    /* +2d6 large */
    WEAPON("katana", "samurai sword",
           0, 0, 0, 4, 40, 80, 10, 12, -2, S, P_KATANA, IRON, HI_METAL),
/* special swords set up for artifacts */
    WEAPON("tsurugi", "long samurai sword",
           0, 0, 1, 0, 60, 500, 16, 8, 2, S, P_TWO_HANDED_SWORD, METAL,
           HI_METAL),
    /* +2d6 large */
    WEAPON("runesword", "runed broadsword",
           0, 0, 0, 0, 40, 300, 4, 6, 0, S, P_BROAD_SWORD, IRON, CLR_BLACK),
    /* +d4 small, +1 large */
    /* +5d2 +d8 from level drain */

/* polearms */
    WEAPON("partisan", "vulgar polearm",
           0, 0, 1, 20, 80, 10, 6, 6, 0, P, P_POLEARMS, IRON, HI_METAL),
    WEAPON("glaive", "single-edged polearm",
           0, 0, 1, 20, 75, 6, 6, 10, 0, S, P_POLEARMS, IRON, HI_METAL),
    WEAPON("lance", NULL,
           1, 0, 0, 8, 180, 10, 6, 8, 0, P, P_POLEARMS, IRON, HI_METAL),
    WEAPON("halberd", "angled poleaxe",
           0, 0, 1, 20, 150, 10, 10, 6, 0, P | S, P_POLEARMS, IRON, HI_METAL),
    WEAPON("dwarvish mattock", "broad pick",
           0, 0, 1, 13, 120, 50, 12, 8, -1, B, P_PICK_AXE, IRON, HI_METAL),

/* bludgeons */
    WEAPON("mace", NULL,
           1, 0, 0, 40, 30, 5, 6, 6, 0, B, P_MACE, IRON, HI_METAL),
    /* +1 small */
    WEAPON("morning star", NULL,
           1, 0, 0, 12, 120, 10, 4, 6, 0, B, P_MORNING_STAR, IRON, HI_METAL),
    /* +d4 small, +1 large */
    WEAPON("war hammer", NULL,
           1, 0, 0, 15, 50, 5, 4, 4, 0, B, P_HAMMER, IRON, HI_METAL),
    /* +1 small */
    WEAPON("club", NULL,
           1, 0, 0, 12, 30, 3, 6, 3, 0, B, P_CLUB, WOOD, HI_WOOD),
    WEAPON("quarterstaff", "staff",
           0, 0, 1, 11, 40, 5, 4, 4, 0, B, P_QUARTERSTAFF, WOOD, HI_WOOD),
/* two-piece */
    WEAPON("aklys", "thonged club",
           0, 0, 0, 8, 15, 4, 6, 3, 0, B, P_CLUB, IRON, HI_METAL),
    WEAPON("flail", NULL,
           1, 0, 0, 40, 15, 4, 6, 4, 0, B, P_FLAIL, IRON, HI_METAL),
    /* +1 small, +1d4 large */
/* misc */
    WEAPON("bullwhip", NULL,
           1, 0, 0, 2, 20, 4, 2, 1, 0, 0, P_WHIP, LEATHER, CLR_BROWN),

/* bows */
    BOW("bow", NULL, 1, 24, 30, 60, 0, WOOD, P_BOW, HI_WOOD),
    BOW("elven bow", "runed bow", 0, 12, 30, 60, 0, WOOD, P_BOW, HI_WOOD),
    BOW("orcish bow", "crude bow", 0, 12, 30, 60, 0, WOOD, P_BOW, CLR_BLACK),
    BOW("yumi", "long bow", 0, 0, 30, 60, 0, WOOD, P_BOW, HI_WOOD),
    BOW("sling", NULL, 1, 40, 3, 20, 0, LEATHER, P_SLING, HI_LEATHER),
    BOW("crossbow", NULL, 1, 45, 50, 40, 0, WOOD, P_CROSSBOW, HI_WOOD),

#undef P
#undef S
#undef B

#undef WEAPON
#undef PROJECTILE
#undef BOW

/* armor ... */
/* IRON denotes ferrous metals, including steel.
 * Only IRON weapons and armor can rust.
 * Only COPPER (including brass), and IRON, corrode.
 * Some creatures are vulnerable to SILVER.
 */
#define ARMOR(name,desc,kn,mgc,blk,power,prob,delay,wt,cost,ac,can, \
              minsz, maxsz, sub, metal, c)                          \
        OBJECT( \
                OBJ(name,desc), BITS(kn,0,1,0,mgc,1,0,0,blk,0,0,sub,metal), \
                power, ARMOR_CLASS, prob, delay, wt, cost, minsz, maxsz, \
                10 - ac, can, wt, c )
/* But some armor provides two extrinsics: */
#define ARMORTWO(name,desc,kn,mgc,blk,pwr1,pwr2,prob,delay,wt,cost,ac,can, \
                 minsz, maxsz, sub, metal, c)                           \
        OBJECT2( \
                OBJ(name,desc), BITS(kn,0,1,0,mgc,1,0,0,blk,0,0,sub,metal), \
                pwr1, pwr2, ARMOR_CLASS, prob, delay, wt, cost, minsz, maxsz, \
                10 - ac, can, wt, c )

#define HELM(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,minsz,maxsz, \
             metal,c)                                                   \
        ARMOR(name,desc,kn,mgc,0,power,prob,delay,wt,cost,ac,can,minsz,maxsz,\
              ARM_HELM,metal,c)
#define CLOAK(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,minsz,maxsz,\
              metal,c)                                                  \
        ARMOR(name,desc,kn,mgc,0,power,prob,delay,wt,cost,ac,can,minsz,maxsz,\
              ARM_CLOAK,metal,c)
#define SHIELD(name,desc,kn,mgc,blk,power,prob,delay,wt,cost,ac,can,minsz,\
               maxsz,metal,c)                                           \
        ARMOR(name,desc,kn,mgc,blk,power,prob,delay,wt,cost,ac,can,minsz,maxsz,\
              ARM_SHIELD, metal,c)
#define GLOVES(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,minsz,maxsz,\
               metal,c)                                                 \
        ARMOR(name,desc,kn,mgc,0,power,prob,delay,wt,cost,ac,can,minsz,maxsz,\
              ARM_GLOVES,metal,c)
#define BOOTS(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,minsz,maxsz,\
              metal,c)                                                  \
        ARMOR(name,desc,kn,mgc,0,power,prob,delay,wt,cost,ac,can,minsz,\
              maxsz,ARM_BOOTS,metal,c)

/* helmets */
    HELM("elven leather helm", "leather hat",
         0, 0, 0, 6, 1, 3, 8, 9, 0, MZ_SMALL, MZ_LARGE, LEATHER, HI_LEATHER),
    HELM("orcish helm", "iron skull cap",
         0, 0, 0, 6, 1, 30, 10, 9, 0, MZ_MEDIUM, MZ_LARGE, IRON, CLR_BLACK),
    HELM("dwarvish iron helm", "hard hat",
         0, 0, 0, 6, 1, 40, 20, 8, 0, MZ_SMALL, MZ_MEDIUM, IRON, HI_METAL),
    HELM("fedora", NULL,
         1, 0, 0, 0, 0, 3, 1, 10, 0, MZ_HUMAN, MZ_HUMAN, CLOTH, CLR_BROWN),
    HELM("cornuthaum", "conical hat",
         0, 1, CLAIRVOYANT,
         3, 1, 4, 80, 10, 2, MZ_SMALL, MZ_HUGE, CLOTH, CLR_BLUE),
    HELM("dunce cap", "conical hat",
         0, 1, 0, 3, 1, 4, 1, 10, 0, MZ_SMALL, MZ_HUGE, CLOTH, CLR_BLUE),
    HELM("dented pot", NULL,
         1, 0, 0, 2, 0, 10, 8, 9, 0, MZ_MEDIUM, MZ_HUGE, IRON, CLR_BLACK),
/* With shuffled appearances... */
    HELM("helmet", "plumed helmet",
         0, 0, 0, 10, 1, 30, 10, 9, 0, MZ_SMALL, MZ_LARGE, IRON, HI_METAL),
    HELM("helm of brilliance", "etched helmet",
         0, 1, 0, 6, 1, 50, 50, 9, 0, MZ_SMALL, MZ_LARGE, IRON, CLR_GREEN),
    HELM("helm of opposite alignment", "crested helmet",
         0, 1, 0, 6, 1, 50, 50, 9, 0, MZ_SMALL, MZ_LARGE, IRON, HI_METAL),
    HELM("helm of telepathy", "visored helmet",
         0, 1, TELEPAT, 2, 1, 50, 50, 9, 0, MZ_SMALL, MZ_LARGE, IRON, HI_METAL),

/* suits of armor */
/*
 * There is code in polyself.c that assumes (1) and (2).
 * There is code in obj.h, objnam.c, mon.c, read.c that assumes (2).
 *
 *      (1) The dragon scale mails and the dragon scales are together.
 *      (2) That the order of the dragon scale mail and dragon scales is the
 *          the same defined in monst.c.
 */
#define DRGN_ARMR(name,mgc,power,cost,ac,color)     \
    ARMOR(name,NULL,1,mgc,1,power,0,5,40,cost,ac,0, \
          MZ_HUMAN,MZ_HUMAN,ARM_SUIT,DRAGON_HIDE,color)
/* 3.4.1: dragon scale mail reclassified as "magic" since magic is
   needed to create them */
      DRGN_ARMR("gray dragon scale mail",    1, ANTIMAGIC,  1200, 1, CLR_GRAY),
      DRGN_ARMR("silver dragon scale mail",  1, REFLECTING, 1200, 1, DRAGON_SILVER),
      DRGN_ARMR("red dragon scale mail",     1, FIRE_RES, 900, 1, CLR_RED),
      DRGN_ARMR("white dragon scale mail",   1, COLD_RES, 900, 1, CLR_WHITE),
      DRGN_ARMR("orange dragon scale mail",  1, SLEEP_RES,   900, 1, CLR_ORANGE),
      DRGN_ARMR("black dragon scale mail",   1, DISINT_RES, 1200, 1, CLR_BLACK),
      DRGN_ARMR("blue dragon scale mail",    1, SHOCK_RES, 900, 1, CLR_BLUE),
      DRGN_ARMR("green dragon scale mail",   1, POISON_RES, 900, 1, CLR_GREEN),
      DRGN_ARMR("yellow dragon scale mail",  1, ACID_RES,   900, 1, CLR_YELLOW),
/* For now, only dragons leave these. */
/* 3.4.1: dragon scales left classified as "non-magic"; they confer
   magical properties but are produced "naturally" */
    DRGN_ARMR("gray dragon scales", 0, ANTIMAGIC, 700, 10, CLR_GRAY),
    DRGN_ARMR("silver dragon scales", 0, REFLECTING, 700, 10, DRAGON_SILVER),
    DRGN_ARMR("red dragon scales", 0, FIRE_RES, 500, 10, CLR_RED),
    /* Red and white, in addition to the listed properties, also
       provide you with the corresponding breath attack. */
    DRGN_ARMR("white dragon scales", 0, COLD_RES,
              500, 10, CLR_WHITE),
    DRGN_ARMR("orange dragon scales", 0, SLEEP_RES,
              500, 10, CLR_ORANGE),
    DRGN_ARMR("black dragon scales", 0, DISINT_RES,
              700, 10, CLR_BLACK),
    DRGN_ARMR("blue dragon scales", 0, SHOCK_RES, 500, 10, CLR_BLUE),
    DRGN_ARMR("green dragon scales", 0, POISON_RES,
              500, 10, CLR_GREEN),
    DRGN_ARMR("yellow dragon scales", 0, ACID_RES,
              500, 10, CLR_YELLOW),
#undef DRGN_ARMR

    /* The order in which armor is defined is relevant for sacrifice_gift()
       when giving armor to weaponless characters. */
    ARMOR("leather jacket", NULL,
          1, 0, 0, 0, 12, 0, 30, 10, 9, 0, MZ_SMALL, MZ_MEDIUM,
          ARM_SUIT, LEATHER, CLR_BLACK),
    ARMOR("leather armor", NULL,
          1, 0, 0, 0, 82, 3, 150, 5, 8, 0, MZ_SMALL, MZ_MEDIUM,
          ARM_SUIT, LEATHER, HI_LEATHER),
    ARMOR("orcish ring mail", "crude ring mail",
          0, 0, 0, 0, 20, 5, 250, 80, 8, 1, MZ_MEDIUM, MZ_LARGE,
          ARM_SUIT, IRON, CLR_BLACK),
    ARMOR("ring mail", NULL,
          1, 0, 0, 0, 72, 5, 250, 100, 7, 0, MZ_SMALL, MZ_LARGE,
          ARM_SUIT, IRON, HI_METAL),
    ARMOR("studded leather armor", NULL,
          1, 0, 0, 0, 72, 3, 200, 15, 7, 1, MZ_SMALL, MZ_LARGE,
          ARM_SUIT, LEATHER, HI_LEATHER),
    ARMOR("scale mail", NULL,
          1, 0, 0, 0, 72, 5, 250, 45, 6, 0, MZ_SMALL, MZ_LARGE,
          ARM_SUIT, IRON, HI_METAL),
    ARMOR("orcish chain mail", "crude chain mail",
          0, 0, 0, 0, 20, 5, 300, 75, 6, 1, MZ_MEDIUM, MZ_LARGE,
          ARM_SUIT, IRON, CLR_BLACK),
    ARMOR("chain mail", NULL,
          1, 0, 0, 0, 72, 5, 300, 75, 5, 1, MZ_SMALL, MZ_LARGE,
          ARM_SUIT, IRON, HI_METAL),
    ARMOR("banded mail", NULL,
          1, 0, 1, 0, 72, 5, 350, 90, 4, 0, MZ_MEDIUM, MZ_LARGE,
          ARM_SUIT, IRON, HI_METAL),
    ARMOR("splint mail", NULL,
          1, 0, 1, 0, 62, 5, 400, 80, 4, 1, MZ_MEDIUM, MZ_LARGE,
          ARM_SUIT, IRON, HI_METAL),

    ARMOR("elven mithril-coat", NULL,
          1, 0, 0, 0, 15, 1, 150, 240, 5, 3, MZ_SMALL, MZ_MEDIUM,
          ARM_SUIT, MITHRIL, HI_METAL),
    ARMOR("dwarvish mithril-coat", NULL,
          1, 0, 0, 0, 10, 1, 150, 240, 4, 3, MZ_SMALL, MZ_MEDIUM,
          ARM_SUIT, MITHRIL, HI_METAL),

    ARMOR("bronze plate mail", NULL,
          1, 0, 1, 0, 25, 5, 500, 400, 1, 0, MZ_MEDIUM, MZ_LARGE,
          ARM_SUIT, COPPER, HI_COPPER),
    ARMOR("plate mail", NULL,
          1, 0, 1, 0, 44, 5, 450, 600, 0, 2, MZ_MEDIUM, MZ_LARGE,
          ARM_SUIT, IRON, HI_METAL),
    ARMOR("crystal plate mail", NULL,
          1, 0, 1, 0, 10, 5, 600, 1000, -1, 3, MZ_MEDIUM, MZ_LARGE,
          ARM_SUIT, GLASS, CLR_WHITE),

/* shirts */
    ARMOR("Hawaiian shirt", NULL,
          1, 0, 0, 0, 8, 0, 5, 3, 10, 0, MZ_SMALL, MZ_MEDIUM,
          ARM_SHIRT, CLOTH, CLR_MAGENTA),
    ARMOR("T-shirt", NULL,
          1, 0, 0, 0, 2, 0, 5, 2, 10, 0, MZ_SMALL, MZ_MEDIUM,
          ARM_SHIRT, CLOTH, CLR_WHITE),

/* cloaks */
/*  'cope' is not a spelling mistake... leave it be */
    CLOAK("mummy wrapping", NULL,
          1, 0, 0, 0, 0, 3, 2, 10, 1, MZ_TINY, MZ_HUGE, CLOTH, CLR_GRAY),
    CLOAK("elven cloak", "faded pall",
          0, 1, STEALTH, 8, 0, 10, 60, 9, 3, MZ_SMALL, MZ_HUGE,
          CLOTH, CLR_BLACK),
    CLOAK("orcish cloak", "coarse mantelet",
          0, 0, 0, 8, 0, 10, 40, 10, 2, MZ_MEDIUM, MZ_HUGE, CLOTH, CLR_BLACK),
    CLOAK("dwarvish cloak", "hooded cloak",
          0, 0, 0, 8, 0, 10, 50, 10, 2, MZ_TINY, MZ_LARGE, CLOTH, HI_CLOTH),
    CLOAK("oilskin cloak", "slippery cloak",
          0, 0, 0, 8, 0, 10, 50, 9, 3, MZ_HUMAN, MZ_HUMAN, CLOTH, HI_CLOTH),
    CLOAK("robe", NULL,
          1, 1, 0, 3, 0, 15, 50, 8, 3, MZ_SMALL, MZ_LARGE, CLOTH, CLR_RED),
    /* Can't use the CLOAK macro for one that provides two properties: */
    ARMORTWO("alchemy smock", "apron",0,1,0,POISON_RES,ACID_RES,
             9,0,10,50,9,1,MZ_TINY,MZ_HUGE, ARM_CLOAK,CLOTH,CLR_WHITE),
    CLOAK("leather cloak", NULL,
          1, 0, 0, 8, 0, 15, 40, 9, 1, MZ_HUMAN, MZ_HUMAN, LEATHER, CLR_BROWN),
/* With shuffled appearances... */
    CLOAK("cloak of protection", "tattered cape",
          0, 1, PROTECTION, 8, 0, 10, 50, 7, 3, MZ_SMALL, MZ_LARGE,
          CLOTH, HI_CLOTH),
    CLOAK("cloak of invisibility", "opera cloak",
          0, 1, INVIS, 10, 0, 10, 60, 9, 2, MZ_SMALL, MZ_LARGE,
          CLOTH, CLR_BRIGHT_MAGENTA),
    CLOAK("cloak of magic resistance", "ornamental cope",
          0, 1, ANTIMAGIC, 2, 0, 10, 60, 9, 3, MZ_SMALL, MZ_LARGE,
          CLOTH, CLR_WHITE),
    CLOAK("cloak of endurance", "dirty rag",
          0, 1, HALF_PHDAM, 1, 0, 100, 300, 9, 0, MZ_SMALL, MZ_LARGE,
          CLOTH, CLR_GREEN),
    CLOAK("cloak of displacement", "piece of cloth",
          0, 1, DISPLACED, 10, 0, 10, 50, 9, 2, MZ_SMALL, MZ_LARGE,
          CLOTH, HI_CLOTH),

/* shields */
    SHIELD("small shield", NULL,
           1, 0, 0, 0, 6, 0, 30, 3, 9, 0, MZ_TINY, MZ_LARGE, WOOD, HI_WOOD),
    SHIELD("elven shield", "blue and green shield",
           0, 0, 0, 0, 2, 0, 40, 7, 8, 0, MZ_SMALL, MZ_HUGE, WOOD, CLR_GREEN),
    SHIELD("Uruk-hai shield", "white-handed shield",
           0, 0, 0, 0, 2, 0, 50, 7, 9, 0, MZ_SMALL, MZ_HUGE, IRON, HI_METAL),
    SHIELD("orcish shield", "red-eyed shield",
           0, 0, 0, 0, 2, 0, 50, 7, 9, 0, MZ_MEDIUM, MZ_HUGE, IRON, CLR_RED),
    SHIELD("large shield", NULL,
           1, 0, 1, 0, 7, 0, 100, 10, 8, 0, MZ_MEDIUM, MZ_HUGE, IRON, HI_METAL),
    SHIELD("dwarvish roundshield", "large round shield",
           0, 0, 0, 0, 4, 0, 100, 10, 8, 0, MZ_SMALL, MZ_HUGE, IRON, HI_METAL),
    SHIELD("shield of reflection", "polished silver shield",
           0, 1, 0, REFLECTING, 3, 0, 50, 50, 8, 0, MZ_SMALL, MZ_HUGE,
           SILVER, HI_SILVER),

/* gloves */
/* these have their color but not material shuffled, so the IRON must stay
 * CLR_BROWN (== HI_LEATHER)
 */
    GLOVES("leather gloves", "old gloves",
           0, 0, 0, 16, 1, 10, 8, 9, 0, MZ_SMALL, MZ_LARGE,
           LEATHER, HI_LEATHER),
    GLOVES("gauntlets of fumbling", "padded gloves",
           0, 1, FUMBLING, 8, 1, 10, 50, 9, 0, MZ_SMALL, MZ_LARGE,
           LEATHER, HI_LEATHER),
    GLOVES("gauntlets of power", "riding gloves",
           0, 1, 0, 8, 1, 10, 50, 9, 0, MZ_SMALL, MZ_LARGE,
           IRON, CLR_BROWN),
    GLOVES("gauntlets of dexterity", "fencing gloves",
           0, 1, 0, 8, 1, 10, 50, 9, 0, MZ_SMALL, MZ_LARGE,
           LEATHER, HI_LEATHER),

/* boots */
    BOOTS("low boots", "walking shoes",
          0, 0, 0, 25, 2, 10, 8, 9, 0, MZ_TINY, MZ_LARGE, LEATHER, HI_LEATHER),
    BOOTS("iron shoes", "hard shoes",
          0, 0, 0, 7, 2, 50, 16, 8, 0, MZ_TINY, MZ_LARGE, IRON, HI_METAL),
    BOOTS("high boots", "jackboots",
          0, 0, 0, 15, 2, 20, 12, 8, 0, MZ_SMALL, MZ_HUGE, LEATHER, HI_LEATHER),
/* With shuffled appearances... */
    BOOTS("speed boots", "combat boots",
          0, 1, FAST, 12, 2, 20, 50, 9, 0, MZ_SMALL, MZ_HUGE,
          LEATHER, HI_LEATHER),
    BOOTS("water walking boots", "jungle boots",
          0, 1, WWALKING, 12, 2, 15, 50, 9, 0, MZ_SMALL, MZ_HUGE,
          LEATHER, HI_LEATHER),
    BOOTS("jumping boots", "hiking boots",
          0, 1, JUMPING, 12, 2, 20, 50, 9, 0, MZ_SMALL, MZ_HUGE,
          LEATHER, HI_LEATHER),
    BOOTS("elven boots", "mud boots",
          0, 1, STEALTH, 12, 2, 15, 8, 9, 0, MZ_SMALL, MZ_HUGE,
          LEATHER, HI_LEATHER),
    BOOTS("kicking boots", "buckled boots",
          0, 1, 0, 12, 2, 50, 8, 9, 0, MZ_SMALL, MZ_HUGE,
          IRON, CLR_BROWN),
    BOOTS("fumble boots", "riding boots",
          0, 1, FUMBLING, 12, 2, 20, 30, 9, 0, MZ_SMALL, MZ_HUGE,
          LEATHER, HI_LEATHER),
    BOOTS("levitation boots", "snow boots",
          0, 1, LEVITATION, 12, 2, 15, 30, 9, 0, MZ_SMALL, MZ_HUGE,
          LEATHER, HI_LEATHER),
#undef HELM
#undef CLOAK
#undef SHIELD
#undef GLOVES
#undef BOOTS
#undef ARMOR

/* rings ... */
#define RING(name,power,stone,cost,mgc,spec,mohs,metal,color) OBJECT( \
           OBJ(name,stone), \
           BITS(0,0,spec,0,mgc,spec,0,0,0,HARDGEM(mohs),0,P_NONE,metal), \
           power, RING_CLASS, 1, 0, 3, cost, 0, 0, 0, 0, 15, color )
    RING("adornment", ADORNED, "wooden", 100, 1, 1, 2, WOOD, HI_WOOD),
    RING("gain strength", 0, "granite", 150, 1, 1, 7, MINERAL, HI_MINERAL),
    RING("gain constitution", 0, "opal", 150, 1, 1, 7, MINERAL, HI_MINERAL),
    RING("increase accuracy", 0, "clay", 150, 1, 1, 4, MINERAL, CLR_RED),
    RING("increase damage", 0, "coral", 150, 1, 1, 4, MINERAL, CLR_ORANGE),
    RING("protection", PROTECTION, "black onyx", 100, 1, 1, 7, MINERAL,
         CLR_BLACK),
    RING("regeneration", REGENERATION, "moonstone",
         200, 1, 0, 6, MINERAL, HI_MINERAL),
    RING("searching", SEARCHING, "tiger eye", 200, 1, 0, 6, GEMSTONE,
         CLR_BROWN),
    RING("stealth", STEALTH, "jade", 100, 1, 1, 6, GEMSTONE, CLR_GREEN),
    RING("sustain ability", FIXED_ABIL, "bronze",
         100, 1, 0, 4, COPPER, HI_COPPER),
    RING("levitation", LEVITATION, "agate", 200, 1, 0, 7, GEMSTONE, CLR_RED),
    RING("hunger", HUNGER, "topaz", 100, 1, 0, 8, GEMSTONE, CLR_CYAN),
    RING("aggravate monster", AGGRAVATE_MONSTER, "sapphire",
         150, 1, 0, 9, GEMSTONE, CLR_BLUE),
    RING("conflict", CONFLICT, "ruby", 300, 1, 0, 9, GEMSTONE, CLR_RED),
    RING("warning", WARNING, "diamond", 100, 1, 0, 10, GEMSTONE, CLR_WHITE),
    RING("poison resistance", POISON_RES, "pearl",
         150, 1, 0, 4, BONE, CLR_WHITE),
    RING("fire resistance", FIRE_RES, "iron", 200, 1, 0, 5, IRON, HI_METAL),
    RING("cold resistance", COLD_RES, "brass", 150, 1, 0, 4, COPPER, HI_COPPER),
    RING("shock resistance", SHOCK_RES, "copper",
         150, 1, 0, 3, COPPER, HI_COPPER),
    RING("free action", FREE_ACTION, "twisted",
         200, 1, 0, 6, IRON, HI_METAL),
    RING("slow digestion", SLOW_DIGESTION, "steel",
         200, 1, 0, 8, IRON, HI_METAL),
    RING("teleportation", TELEPORT, "silver", 200, 1, 0, 3, SILVER, HI_SILVER),
    RING("teleport control", TELEPORT_CONTROL, "gold",
         300, 1, 0, 3, GOLD, HI_GOLD),
    RING("polymorph", POLYMORPH, "ivory", 300, 1, 0, 4, BONE, CLR_WHITE),
    RING("polymorph control", POLYMORPH_CONTROL, "emerald",
         300, 1, 0, 8, GEMSTONE, CLR_BRIGHT_GREEN),
    RING("invisibility", INVIS, "wire", 150, 1, 0, 5, IRON, HI_METAL),
    RING("see invisible", SEE_INVIS, "engagement",
         150, 1, 0, 5, IRON, HI_METAL),
    RING("protection from shape changers", PROT_FROM_SHAPE_CHANGERS, "shiny",
         100, 1, 0, 5, IRON, CLR_BRIGHT_CYAN),
#undef RING
/* Spare appearances should have 0 probability: */
#define RING(name,power,stone,cost,mgc,spec,mohs,metal,color) OBJECT(   \
           OBJ(name,stone), \
           BITS(0,0,spec,0,mgc,spec,0,0,0,HARDGEM(mohs),0,P_NONE,metal), \
           power, RING_CLASS, 0, 0, 3, cost, 0, 0, 0, 0, 15, color )
    RING(NULL, 0, "dime store", 100, 1, 0,  0, PLASTIC, CLR_WHITE),
    RING(NULL, 0, "plain",      100, 1, 0,  4, IRON, HI_METAL),
    RING(NULL, 0, "ridged",     100, 1, 0,  5, IRON, HI_METAL),
    RING(NULL, 0, "glass",      100, 1, 0,  5, GLASS, CLR_BRIGHT_CYAN),
    RING(NULL, 0, "obsidian",   100, 1, 0,  6, MINERAL, CLR_BLACK),
    RING(NULL, 0, "tin",        100, 1, 0,  2, METAL, HI_METAL),
    RING(NULL, 0, "bone",       100, 1, 0,  4, BONE, CLR_WHITE),
    RING(NULL, 0, "ceramic",    100, 1, 0,  6, MINERAL, CLR_BROWN),
    RING(NULL, 0, "promise",    100, 1, 0,  4, IRON, CLR_BRIGHT_GREEN),
    RING(NULL, 0, "mood",       100, 1, 0,  3, METAL, CLR_MAGENTA),
    RING(NULL, 0, "signet",     100, 1, 0,  5, COPPER, HI_COPPER),
    RING(NULL, 0, "scarab",     100, 1, 0,  3, GOLD, HI_GOLD),
    RING(NULL, 0, "intaglio",   100, 1, 0,  4, IRON, CLR_ORANGE),
    RING(NULL, 0, "class",      100, 1, 0,  2, METAL, CLR_GREEN),
    RING(NULL, 0, "wedding",    100, 1, 0, 10, GOLD, HI_GOLD),
    RING(NULL, 0, "puzzle",     100, 1, 0,  2, WOOD, CLR_BROWN),
#undef RING

/* amulets ... - THE Amulet comes last because it is special */
#define AMULET(name,desc,power,prob) OBJECT( \
           OBJ(name,desc), BITS(0,0,0,0,1,0,0,0,0,0,0,P_NONE,IRON), power, \
           AMULET_CLASS, prob, 0, 20, 150, 0, 0, 0, 0, 20, HI_METAL )

    AMULET("amulet of ESP", "circular", TELEPAT, 175),
    AMULET("amulet of life saving", "spherical", LIFESAVED, 75),
    AMULET("amulet of strangulation", "oval", STRANGLED, 135),
    AMULET("amulet of restful sleep", "triangular", SLEEPING, 135),
    AMULET("amulet versus poison", "pyramidal", POISON_RES, 150),
    AMULET("amulet of change", "square", 0, 124),
    /* POLYMORPH */
    AMULET("amulet of unchanging", "concave", UNCHANGING, 45),
    AMULET("amulet of reflection", "hexagonal", REFLECTING, 75),
    AMULET("amulet of flying", "convex", FLYING, 20),
    AMULET("amulet of magical breathing", "octagonal", MAGICAL_BREATHING, 65),
    OBJECT(OBJ("cheap plastic imitation of the Amulet of Yendor",
               "Amulet of Yendor"), BITS(0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                         PLASTIC), 0,
           AMULET_CLASS, 1, 0, 20, 0, 0, 0, 0, 0, 1, HI_METAL),
    OBJECT(OBJ("Amulet of Yendor",      /* note: description == name */
               "Amulet of Yendor"), BITS(0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0,
                                         MITHRIL), 0,
           AMULET_CLASS, 0, 0, 20, 30000, 0, 0, 0, 0, 20, HI_METAL),
#undef AMULET

/* tools ... */
/* tools with weapon characteristics come last */
#define TOOLX(name,desc,kn,mrg,mgc,chg,prob,wt,cost,mat,color,power)    \
    OBJECT( OBJ(name,desc),                                             \
            BITS(kn,mrg,chg,0,mgc,chg,0,0,0,0,0,P_NONE,mat),            \
            power, TOOL_CLASS, prob, 0,                                 \
            wt, cost, 0, 0, 0, 0, wt, color )
#define TOOL(name,desc,kn,mrg,mgc,chg,prob,wt,cost,mat,color)   \
    TOOLX(name,desc,kn,mrg,mgc,chg,prob,wt,cost,mat,color,0)
#define LIGHTSOURCE(name,desc,kn,mrg,mgc,chg,prob,wt,cost,mat,color)    \
    OBJECT( OBJ(name,desc),                                             \
            BITS(kn,mrg,1,0,mgc,chg,0,0,0,0,0,P_NONE,mat),              \
            0, TOOL_CLASS, prob, 0,                                     \
            wt, cost, 0, 0, 0, 0, wt, color )
#define CONTAINER(name,desc,kn,mgc,chg,prob,wt,cost,mat,color)  \
    OBJECT( OBJ(name,desc),                                     \
            BITS(kn,0,chg,1,mgc,chg,0,0,0,0,0,P_NONE,mat),      \
            0, TOOL_CLASS, prob, 0,                             \
            wt, cost, 0, 0, 0, 0, wt, color )
#define WEPTOOL(name,desc,kn,mgc,bi,prob,wt,cost,sdam,ldam,hitbon,sub,mat,clr) \
    OBJECT( OBJ(name,desc),                                             \
            BITS(kn,0,1,0,mgc,1,0,0,bi,0,hitbon,sub,mat),               \
            0, TOOL_CLASS, prob, 0,                                     \
            wt, cost, sdam, ldam, hitbon, 0, wt, clr )
/* containers */
    CONTAINER("large box", NULL, 1, 0, 0, 40, 350, 8, WOOD, HI_WOOD),
    CONTAINER("chest", NULL, 1, 0, 0, 35, 600, 16, WOOD, HI_WOOD),
    CONTAINER("ice box", NULL, 1, 0, 0, 5, 900, 42, PLASTIC, CLR_WHITE),
    CONTAINER("sack", "bag", 0, 0, 0, 35, 15, 2, CLOTH, HI_CLOTH),
    CONTAINER("oilskin sack", "bag", 0, 0, 0, 5, 15, 100, CLOTH, HI_CLOTH),
    CONTAINER("bag of holding", "bag", 0, 1, 0, 20, 15, 100, CLOTH, HI_CLOTH),
    CONTAINER("bag of tricks", "bag", 0, 1, 1, 20, 15, 100, CLOTH, HI_CLOTH),
#undef CONTAINER

/* lock opening tools */
    TOOL("brass key", NULL, 0, 0, 0, 0, 15, 3, 20, COPPER, HI_COPPER),
    TOOL("silver key", NULL, 0, 0, 0, 0, 10, 3, 40, SILVER, HI_SILVER),
    TOOL("bronze key", NULL, 0, 0, 0, 0, 15, 4, 15, COPPER, HI_COPPER),
    TOOL("door key", "key", 0, 0, 0, 0, 10, 3, 10, METAL, HI_METAL),
    TOOL("skeleton key", NULL, 0, 0, 0, 0, 10, 2, 10, BONE, CLR_WHITE),
    TOOL("iron key", "key", 0, 0, 0, 0, 15, 3, 10, IRON, HI_METAL),
    TOOL("sturdy key", "key", 0, 0, 0, 0,  5, 2, 100, MITHRIL, HI_METAL),
    TOOL("lock pick", NULL, 1, 0, 0, 0, 60, 4, 20, IRON, HI_METAL),
    TOOL("credit card", NULL, 1, 0, 0, 0, 15, 1, 10, PLASTIC, CLR_WHITE),
/* light sources */
    LIGHTSOURCE("tallow candle", "candle", 0, 1, 0, 0, 20, 2, 10, WAX,
                CLR_WHITE),
    LIGHTSOURCE("wax candle", "candle", 0, 1, 0, 0, 5, 2, 20, WAX, CLR_WHITE),
    LIGHTSOURCE("brass lantern", NULL, 1, 0, 0, 0, 30, 30, 12, COPPER,
                CLR_YELLOW),
    LIGHTSOURCE("oil lamp", "lamp", 0, 0, 0, 0, 45, 20, 10, COPPER, CLR_YELLOW),
/* magic lamps can't run out, so are marked as TOOL instead */
    TOOL("magic lamp", "lamp", 0, 0, 1, 0, 15, 20, 50, COPPER, CLR_YELLOW),
/* other tools */
    TOOL("expensive camera", NULL,
         1, 0, 0, 1, 15, 12, 200, PLASTIC, CLR_BLACK),
    TOOL("mirror", "looking glass", 0, 0, 0, 0, 45, 13, 10, GLASS, HI_SILVER),
    TOOL("crystal ball", "glass orb",
         0, 0, 1, 1, 15, 150, 60, GLASS, HI_GLASS),
    TOOLX("lenses", NULL, 1, 0, 0, 0, 5, 3, 80, GLASS, HI_GLASS, SEARCHING),
    TOOLX("blindfold", NULL, 1, 0, 0, 0, 50, 2, 20, CLOTH, CLR_BLACK, BLINDED),
    TOOLX("towel", NULL, 1, 0, 0, 0, 50, 2, 50, CLOTH, CLR_MAGENTA, BLINDED),
    TOOL("saddle", NULL, 1, 0, 0, 0, 5, 200, 150, LEATHER, HI_LEATHER),
    TOOL("leash", NULL, 1, 0, 0, 0, 65, 12, 20, LEATHER, HI_LEATHER),
    TOOL("stethoscope", NULL, 1, 0, 0, 0, 25, 4, 75, IRON, HI_METAL),
    TOOL("tinning kit", NULL, 1, 0, 0, 1, 15, 100, 30, IRON, HI_METAL),
    TOOL("tin opener", NULL, 1, 0, 0, 0, 35, 4, 30, IRON, HI_METAL),
    TOOL("can of grease", NULL, 1, 0, 0, 1, 15, 15, 20, IRON, HI_METAL),
    TOOL("figurine", NULL, 1, 0, 1, 0, 25, 50, 80, MINERAL, HI_MINERAL),
    TOOL("magic marker", NULL, 1, 0, 1, 1, 15, 2, 50, PLASTIC, CLR_RED),
/* traps */
    TOOL("land mine", "explosive trap", 0, 0, 0, 0, 0, 300, 180, IRON, CLR_RED),
    TOOL("beartrap", "toothed trap", 0, 0, 0, 0, 0, 200, 60, IRON, HI_METAL),
/* instruments */
    TOOL("tin whistle", "whistle", 0, 0, 0, 0, 100, 3, 10, METAL, HI_METAL),
    TOOL("magic whistle", "whistle", 0, 0, 1, 0, 30, 3, 10, METAL, HI_METAL),
/* "If tin whistles are made out of tin, what do they make foghorns out of?" */
    TOOL("wooden flute", "flute", 0, 0, 0, 0, 4, 5, 12, WOOD, HI_WOOD),
    TOOL("magic flute", "flute", 0, 0, 1, 1, 2, 5, 36, WOOD, HI_WOOD),
    TOOL("tooled horn", "horn", 0, 0, 0, 0, 5, 18, 15, BONE, CLR_WHITE),
    TOOL("frost horn", "horn", 0, 0, 1, 1, 2, 18, 50, BONE, CLR_WHITE),
    TOOL("fire horn", "horn", 0, 0, 1, 1, 2, 18, 50, BONE, CLR_WHITE),
    TOOL("horn of plenty", "horn", 0, 0, 1, 1, 2, 18, 50, BONE, CLR_WHITE),
    TOOL("wooden harp", "harp", 0, 0, 0, 0, 4, 30, 50, WOOD, HI_WOOD),
    TOOL("magic harp", "harp", 0, 0, 1, 1, 2, 30, 50, WOOD, HI_WOOD),
    TOOL("bell", NULL, 1, 0, 0, 0, 2, 30, 50, COPPER, HI_COPPER),
    TOOL("bugle", NULL, 1, 0, 0, 0, 4, 10, 15, COPPER, HI_COPPER),
    TOOL("leather drum", "drum", 0, 0, 0, 0, 4, 25, 25, LEATHER, HI_LEATHER),
    TOOL("drum of earthquake", "drum",
         0, 0, 1, 1, 2, 25, 25, LEATHER, HI_LEATHER),
/* tools useful as weapons */
    WEPTOOL("pick-axe", NULL,
            1, 0, 0, 20, 100, 50, 6, 3, WHACK, P_PICK_AXE, IRON, HI_METAL),
    WEPTOOL("grappling hook", "iron hook",
            0, 0, 0, 5, 30, 50, 2, 6, WHACK, P_FLAIL, IRON, HI_METAL),
/* 3.4.1: unicorn horn left classified as "magic" */
    WEPTOOL("unicorn horn", NULL,
            1, 1, 1, 0,  8, 100, 12, 12, PIERCE, P_UNICORN_HORN, BONE,
            CLR_WHITE),

/* two special unique artifact "tools" */
    OBJECT(OBJ("Candelabrum of Invocation", "candelabrum"),
           BITS(0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, P_NONE, GOLD), 0,
           TOOL_CLASS, 0, 0, 10, 5000, 0, 0, 0, 0, 200, HI_GOLD),
    OBJECT(OBJ("Bell of Opening", "silver bell"),
           BITS(0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, P_NONE, SILVER), 0,
           TOOL_CLASS, 0, 0, 10, 5000, 0, 0, 0, 0, 50, HI_SILVER),
#undef TOOL
#undef WEPTOOL

/* Comestibles ... */
#define FOOD(name,prob,delay,wt,unk,tin,nutrition,color) OBJECT( \
        OBJ(name,NULL), BITS(1,1,unk,0,0,0,0,0,0,0,0,P_NONE,tin), 0, \
           FOOD_CLASS, prob, delay, \
           wt, nutrition/20 + 5, 0, 0, 0, 0, nutrition, color )
/* all types of food (except tins & corpses) must have a delay of at least 1. */
/* delay on corpses is computed and is weight dependant */
/* dog eats foods 0-4 but prefers tripe rations above all others */
/* fortune cookies can be read */
/* carrots improve your vision */
/* +0 tins contain monster meat */
/* +1 tins (of spinach) make you stronger (like Popeye) */
/* food CORPSE is a cadaver of some type */
/* meatballs/sticks/rings are only created from objects via stone to flesh */

/* meat */
    FOOD("tripe ration", 140, 2, 10, 0, FLESH, 200, CLR_BROWN),
    FOOD("corpse", 0, 1, 0, 0, FLESH, 0, CLR_BROWN),
    FOOD("egg", 85, 1, 1, 1, FLESH, 80, CLR_WHITE),
    FOOD("meatball", 0, 1, 1, 0, FLESH, 5, CLR_BROWN),
    FOOD("meat stick", 0, 1, 1, 0, FLESH, 5, CLR_BROWN),
    FOOD("huge chunk of meat", 0, 20, 400, 0, FLESH, 2000, CLR_BROWN),
/* special case because it's not mergable */
    OBJECT(OBJ("meat ring", NULL),
           BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, FLESH),
           0, FOOD_CLASS, 0, 1, 5, 1, 0, 0, 0, 0, 5, CLR_BROWN),

/* fruits & veggies */
    FOOD("kelp frond", 0, 1, 1, 0, VEGGY, 30, CLR_GREEN),
    FOOD("eucalyptus leaf", 3, 1, 1, 0, VEGGY, 30, CLR_GREEN),
    FOOD("apple", 15, 1, 2, 0, VEGGY, 50, CLR_RED),
    FOOD("orange", 14, 1, 2, 0, VEGGY, 80, CLR_ORANGE),
    FOOD("pear", 12, 1, 2, 0, VEGGY, 50, CLR_BRIGHT_GREEN),
    FOOD("banana", 12, 1, 2, 0, VEGGY, 80, CLR_YELLOW),
    FOOD("carrot", 15, 1, 2, 0, VEGGY, 50, CLR_ORANGE),
    FOOD("sprig of wolfsbane", 8, 1, 1, 0, VEGGY, 40, CLR_GREEN),
    FOOD("clove of garlic", 8, 1, 1, 0, VEGGY, 40, CLR_WHITE),
    FOOD("slime mold", 75, 1, 5, 0, VEGGY, 250, HI_ORGANIC),

/* people food */
    FOOD("lump of royal jelly", 0, 1, 2, 0, VEGGY, 200, CLR_YELLOW),
    FOOD("cream pie", 25, 1, 10, 0, VEGGY, 100, CLR_WHITE),
    FOOD("candy bar", 13, 1, 2, 0, VEGGY, 100, CLR_BROWN),
    FOOD("fortune cookie", 55, 1, 1, 0, VEGGY, 40, CLR_YELLOW),
    FOOD("pancake", 25, 2, 2, 0, VEGGY, 200, CLR_YELLOW),
    FOOD("lembas wafer", 20, 2, 5, 0, VEGGY, 800, CLR_WHITE),
    FOOD("cram ration", 20, 3, 15, 0, VEGGY, 600, HI_ORGANIC),
    FOOD("food ration", 380, 5, 20, 0, VEGGY, 800, HI_ORGANIC),
    FOOD("K-ration", 0, 1, 10, 0, VEGGY, 400, HI_ORGANIC),
    FOOD("C-ration", 0, 1, 10, 0, VEGGY, 300, HI_ORGANIC),
    FOOD("tin", 75, 0, 10, 1, METAL, 0, HI_METAL),
#undef FOOD

/* potions ... */
#define POTION(name,desc,mgc,power,prob,cost,color) OBJECT( \
        OBJ(name,desc), BITS(0,1,0,0,mgc,0,0,0,0,0,0,P_NONE,GLASS), power, \
            POTION_CLASS, prob, 0, 20, cost, 0, 0, 0, 0, 10, color )
    POTION("gain ability", "ruby", 1, 0, 42, 300, CLR_RED),
    POTION("restore ability", "pink", 1, 0, 40, 100, CLR_BRIGHT_MAGENTA),
    POTION("confusion", "orange", 1, CONFUSION, 42, 100, CLR_ORANGE),
    POTION("blindness", "yellow", 1, BLINDED, 40, 150, CLR_YELLOW),
    POTION("paralysis", "emerald", 1, 0, 42, 300, CLR_BRIGHT_GREEN),
    POTION("speed", "dark green", 1, FAST, 42, 200, CLR_GREEN),
    POTION("levitation", "cyan", 1, LEVITATION, 42, 200, CLR_CYAN),
    POTION("hallucination", "sky blue", 1, HALLUC, 40, 100, CLR_CYAN),
    POTION("invisibility", "brilliant blue", 1, INVIS, 40, 150,
           CLR_BRIGHT_BLUE),
    POTION("sight", "magenta", 1, SEE_INVIS, 42, 50, CLR_MAGENTA),
    POTION("healing", "purple-red", 1, 0, 57, 100, CLR_MAGENTA),
    POTION("extra healing", "puce", 1, 0, 47, 100, CLR_RED),
    POTION("gain level", "milky", 1, 0, 20, 300, CLR_WHITE),
    POTION("enlightenment", "swirly", 1, 0, 20, 200, CLR_BROWN),
    POTION("monster detection", "bubbly", 1, 0, 40, 150, CLR_WHITE),
    POTION("object detection", "smoky", 1, 0, 42, 150, CLR_GRAY),
    POTION("gain energy", "cloudy", 1, 0, 42, 150, CLR_WHITE),
    POTION("sleeping", "effervescent", 1, 0, 42, 100, CLR_GRAY),
    POTION("full healing", "black", 1, 0, 10, 200, CLR_BLACK),
    POTION("polymorph", "golden", 1, 0, 10, 200, CLR_YELLOW),
    POTION("booze", "brown", 0, 0, 42, 50, CLR_BROWN),
    POTION("sickness", "fizzy", 0, 0, 42, 50, CLR_CYAN),
    POTION("fruit juice", "dark", 0, 0, 42, 50, CLR_BLACK),
    POTION("acid", "white", 0, 0, 10, 250, CLR_WHITE),
    POTION("oil", "murky", 0, 0, 30, 250, CLR_BROWN),
    /* Extra appearances: */
    POTION(NULL, "viscous",   1, 0, 0, 100, CLR_GREEN),
    POTION(NULL, "aromatic",  1, 0, 0, 100, CLR_BRIGHT_BLUE),
    POTION(NULL, "sticky",    1, 0, 0, 100, CLR_RED),
    POTION(NULL, "muddy",     1, 0, 0, 100, CLR_BROWN),
    POTION(NULL, "icy",       1, 0, 0, 100, CLR_BRIGHT_CYAN),
    POTION(NULL, "foaming",   1, 0, 0, 100, CLR_BRIGHT_CYAN),
    POTION(NULL, "creamy",    1, 0, 0, 100, CLR_YELLOW),
    POTION(NULL, "gray",      1, 0, 0, 100, CLR_GRAY),
    POTION(NULL, "amber",     1, 0, 0, 100, CLR_ORANGE),
    POTION(NULL, "violet",    1, 0, 0, 100, CLR_BRIGHT_MAGENTA),
    POTION(NULL, "deep blue", 1, 0, 0, 100, CLR_BLUE),
    /* Non-random-appearance (special-cased in shuffle_all): */
    POTION("water", "clear", 0, 0, 92, 100, CLR_CYAN),
#undef POTION

/* scrolls ... */
#define SCROLL(name,text,mgc,prob,cost) OBJECT( \
        OBJ(name,text), BITS(0,1,0,0,mgc,0,0,0,0,0,0,P_NONE,PAPER), 0, \
            SCROLL_CLASS, prob, 0, 5, cost, 0, 0, 0, 0, 6, HI_PAPER )
    /* Cheap scrolls with effectively unique prices: */
    SCROLL("identify", "KERNOD WEL", 1, 180, 20),
    SCROLL("light", "VERR YED HORRE", 1, 90, 50),
    SCROLL("enchant weapon", "DAIYEN FOOELS", 1, /* 75 */ 80, 60),
    /* 80zm */
    SCROLL("enchant armor", "ZELGO MER", 1, 63, 80),
    SCROLL("remove curse", "PRATYAVAYAH", 1, 65, 80),
    /* 100zm */
    SCROLL("destroy armor", "JUYED AWK YACC", 1, 45, 100),
    SCROLL("confuse monster", "NR 9", 1, 53, 100),
    SCROLL("scare monster", "XIXAXA XOXAXA XUXAXA", 1, 50, 100),
    SCROLL("teleportation", "VENZAR BORGAVVE", 1, 55, 100),
    SCROLL("gold detection", "THARR", 1, 40, 100),
    SCROLL("magic mapping", "ELAM EBOW", 1, 45, 100),
    /* 200 zm */
    SCROLL("create monster", "LEP GEX VEN ZEA", 1, 45, 200),
    SCROLL("taming", "PRIRUTSENIE", 1, 15, 200),
    SCROLL("water", "DUAM XNAHT", 1, 35, 200),
    SCROLL("fire", "ANDOVA BEGARIN", 1, 30, 200),
    SCROLL("earth", "KIRJE", 1, 18, 200),
    /* 250zm */
    SCROLL("silence", "FOOBIE BLETCH", 1, /* 5 */0, 250),
    SCROLL("stinking cloud", "VELOX NEB", 1, 18, 250),
    /* 300zm */
    SCROLL("genocide", "ELBIB YLOH", 1, 15, 300),
    SCROLL("punishment", "VE FORBRYDERNE", 1, 15, 300),
    SCROLL("charging", "HACKEM MUCHE", 1, 15, 300),
    SCROLL("consecration", "YUM YUM", 1, 0, 300),
    //SCROLL("wishing", "RATSANOPU", 1, 0, 9000),
    OBJECT(OBJ("wishing","RATSANOPU"),
           BITS(0,1,0,0,1,0,0,1,0,0,0,P_NONE,PAPER),
           0, SCROLL_CLASS, 0, 0, 5, 9000,
           0, 0, 0, 0, 6, HI_PAPER ),
    SCROLL(NULL, "TEMOV", 1, 0, 100),
    SCROLL(NULL, "GARVEN DEH", 1, 0, 100),
    SCROLL(NULL, "READ ME", 1, 0, 100),
    SCROLL(NULL, "ETAOIN SHRDLU", 1, 0, 100),
    SCROLL(NULL, "LOREM IPSUM SIT AMET", 1, 0, 100),
    SCROLL(NULL, "ABRA KA DABRA", 1, 0, 100),
    SCROLL(NULL, "HAPAX LEGOMENON", 1, 0, 100),
    SCROLL(NULL, "XYZZY", 1, 0, 100),
    SCROLL(NULL, "EDEVESH HANAGASH", 1, 0, 100),
    SCROLL(NULL, "EIGOWA KIRAIDES", 1, 0, 100),
    SCROLL(NULL, "FROBOZZ MAGIC SCROLL CO", 1, 0, 100),
    SCROLL(NULL, "PATENT PENDING", 1, 0, 100),
    /* these must come last because they have special descriptions */
    SCROLL("blank paper", "unlabeled", 0, 28, 60),
#undef SCROLL

/*
 * spellbooks ...
 * note: the number of "ordinary" spellbooks must not exceed MAXSPELL-SPID_COUNT
 * (otherwise we have problems assigning letters); each spell has a distinct
 * default letter that's either lowercase, or an uppercase letter that isn't in
 * the last SPID_COUNT (those letters are reserved for supernatural abilities)
 *
 * Currently used: aABcCdDeEfFGhHiIjkKlLmMnoOpPrRsStTuUvwxz
 *         unused: bgJNqQy
 *       reserved: VWXYZ
 *
 * Reasoning:
 * - most spells get the first letter of their name
 * - in case of clashes, we use the first letter of a subsequent word, if any
 * - if there are still clashes (e.g. 'c' spells, 'detect *', 1-word spells)
 *   we use a later letter from the word, or a letter that fits thematically
 *   (e.g. z - sleep)
 * - force bolt gets 'a' because that's where it is for starting wizards in all
 *   previous versions
 * - effects that are more likely to be spammed get first pick of the lowercase
 *   letters
 *
 * The user can override these letters in-game with the + command.
 */
#define SPELL(name,desc,sub,prob,delay,level,mgc,dir,color,deflet)      \
    OBJECT( OBJ(name,desc), BITS(0,0,0,0,mgc,0,0,0,0,0,dir,sub,PAPER), 0, \
            SPBOOK_CLASS, prob, delay,                                  \
            50, level*100, 0, 0, deflet, level, 20, color )
    SPELL("dig", "parchment", P_MATTER_SPELL, 20, 6, 2, 1,
          RAY, HI_PAPER, 'd'),
    SPELL("magic missile", "vellum", P_ATTACK_SPELL, 45, 2, 2, 1,
          RAY, HI_PAPER, 'm'),
    SPELL("fireball", "ragged", P_ATTACK_SPELL, 20, 4, 4, 1,
          RAY, HI_PAPER, 'f'),
    SPELL("cone of cold", "dog eared", P_ATTACK_SPELL, 10, 7, 4, 1,
          RAY, HI_PAPER, 'o'),
    SPELL("sleep", "mottled", P_ENCHANTMENT_SPELL, 50, 1, 2, 1,
          RAY, HI_PAPER, 'z'),
    SPELL("finger of death", "stained", P_ATTACK_SPELL, 5, 10, 7, 1,
          RAY, HI_PAPER, 'F'),
    SPELL("light", "cloth", P_DIVINATION_SPELL, 45, 1, 1, 1,
          NODIR, HI_CLOTH, 'l'),
    SPELL("detect monsters", "leather-bound", P_DIVINATION_SPELL, 43, 1, 1, 1,
          NODIR, HI_LEATHER, 'n'),
    SPELL("healing", "white", P_HEALING_SPELL, 40, 2, 1, 1,
          IMMEDIATE, CLR_WHITE, 'h'),
    SPELL("knock", "pink", P_MATTER_SPELL, 35, 1, 1, 1,
          IMMEDIATE, CLR_BRIGHT_MAGENTA, 'k'),
    SPELL("force bolt", "red", P_ATTACK_SPELL, 35, 2, 1, 1,
          IMMEDIATE, CLR_RED, 'a'),
    SPELL("confuse monster", "orange", P_ENCHANTMENT_SPELL, 30, 2, 2, 1,
          IMMEDIATE, CLR_ORANGE, 'u'),
    SPELL("cure blindness", "yellow", P_HEALING_SPELL, 25, 2, 2, 1,
          IMMEDIATE, CLR_YELLOW, 'B'),
    SPELL("drain life", "velvet", P_ATTACK_SPELL, 10, 2, 3, 1,
          IMMEDIATE, CLR_MAGENTA, 'D'),
    SPELL("slow monster", "light green", P_ENCHANTMENT_SPELL, 30, 2, 1, 1,
          IMMEDIATE, CLR_BRIGHT_GREEN, 's'),
    SPELL("wizard lock", "dark green", P_MATTER_SPELL, 30, 3, 2, 1,
          IMMEDIATE, CLR_GREEN, 'w'),
    SPELL("create monster", "turquoise", P_CLERIC_SPELL, 35, 3, 2, 1,
          NODIR, CLR_BRIGHT_CYAN, 'O'),
    SPELL("detect food", "cyan", P_DIVINATION_SPELL, 30, 3, 2, 1,
          NODIR, CLR_CYAN, 'E'),
    SPELL("cause fear", "light blue", P_ENCHANTMENT_SPELL, 25, 3, 3, 1,
          NODIR, CLR_BRIGHT_BLUE, 'c'),
    SPELL("clairvoyance", "dark blue", P_DIVINATION_SPELL, 15, 3, 3, 1,
          NODIR, CLR_BLUE, 'v'),
    SPELL("cure sickness", "indigo", P_HEALING_SPELL, 32, 3, 3, 1,
          NODIR, CLR_BLUE, 'K'),
    SPELL("charm monster", "magenta", P_ENCHANTMENT_SPELL, 20, 3, 5, 1,
          IMMEDIATE, CLR_MAGENTA, 'e'),
    SPELL("haste self", "purple", P_ESCAPE_SPELL, 33, 4, 3, 1,
          NODIR, CLR_MAGENTA, 'H'),
    SPELL("detect unseen", "violet", P_DIVINATION_SPELL, 20, 4, 3, 1,
          NODIR, CLR_MAGENTA, 'U'),
    SPELL("levitation", "tan", P_ESCAPE_SPELL, 20, 4, 4, 1,
          NODIR, CLR_BROWN, 'L'),
    SPELL("extra healing", "plaid", P_HEALING_SPELL, 27, 5, 3, 1,
          IMMEDIATE, CLR_GREEN, 'x'),
    SPELL("restore ability", "light brown", P_HEALING_SPELL, 25, 5, 4, 1,
          NODIR, CLR_BROWN, 'R'),
    SPELL("invisibility", "dark brown", P_ESCAPE_SPELL, 25, 5, 4, 1,
          NODIR, CLR_BROWN, 'i'),
    SPELL("detect treasure", "gray", P_DIVINATION_SPELL, 20, 5, 4, 1,
          NODIR, CLR_GRAY, 'G'),
    SPELL("remove curse", "wrinkled", P_CLERIC_SPELL, 25, 5, 3, 1,
          NODIR, HI_PAPER, 'r'),
    SPELL("magic mapping", "dusty", P_DIVINATION_SPELL, 18, 7, 5, 1,
          NODIR, HI_PAPER, 'M'),
    SPELL("identify", "bronze", P_DIVINATION_SPELL, 20, 6, 3, 1,
          NODIR, HI_COPPER, 'I'),
    SPELL("turn undead", "copper", P_CLERIC_SPELL, 16, 8, 4, 1,
          IMMEDIATE, HI_COPPER, 'T'),
    SPELL("polymorph", "silver", P_MATTER_SPELL, 10, 8, 6, 1,
          IMMEDIATE, HI_SILVER, 'P'),
    SPELL("teleport away", "gold", P_ESCAPE_SPELL, 15, 6, 6, 1,
          IMMEDIATE, HI_GOLD, 't'),
    SPELL("create familiar", "glittering", P_CLERIC_SPELL, 10, 7, 6, 1,
          NODIR, CLR_WHITE, 'A'),
    SPELL("cancellation", "shining", P_MATTER_SPELL, 15, 8, 7, 1,
          IMMEDIATE, CLR_WHITE, 'C'),
    SPELL("protection", "dull", P_CLERIC_SPELL, 18, 3, 1, 1,
          NODIR, CLR_GRAY, 'p'),
    SPELL("jumping", "thin", P_ESCAPE_SPELL, 20, 3, 1, 1,
          IMMEDIATE, HI_PAPER, 'j'),
    SPELL("stone to flesh", "thick", P_HEALING_SPELL, 15, 1, 3, 1,
          IMMEDIATE, HI_PAPER, 'S'),
/* spare spellbook appearances */
    SPELL(NULL, "dark",         0, 0, 0, 0, 1, 0, CLR_BLACK, '_'),
    SPELL(NULL, "black",        0, 0, 0, 0, 1, 0, CLR_BLACK, '_'),
    SPELL(NULL, "wide",         0, 0, 0, 0, 1, 0, CLR_RED, '_'),
    SPELL(NULL, "long",         0, 0, 0, 0, 1, 0, CLR_ORANGE, '_'),
    SPELL(NULL, "spotted",      0, 0, 0, 0, 1, 0, CLR_ORANGE, '_'),
    SPELL(NULL, "striped",      0, 0, 0, 0, 1, 0, CLR_RED, '_'),
    SPELL(NULL, "spiral-bound", 0, 0, 0, 0, 1, 0, CLR_BRIGHT_BLUE, '_'),
    SPELL(NULL, "tattered",     0, 0, 0, 0, 1, 0, CLR_CYAN, '_'),
    SPELL(NULL, "torn",         0, 0, 0, 0, 1, 0, CLR_CYAN, '_'),
    SPELL(NULL, "calligraphed", 0, 0, 0, 0, 1, 0, CLR_BRIGHT_CYAN, '_'),
    SPELL(NULL, "scrawled",     0, 0, 0, 0, 1, 0, CLR_BRIGHT_CYAN, '_'),
    SPELL(NULL, "hand-printed", 0, 0, 0, 0, 1, 0, CLR_BRIGHT_CYAN, '_'),
    SPELL(NULL, "woodcut",      0, 0, 0, 0, 1, 0, CLR_GRAY, '_'),
    SPELL(NULL, "embossed",     0, 0, 0, 0, 1, 0, CLR_BLACK, '_'),
    SPELL(NULL, "illuminated",  0, 0, 0, 0, 1, 0, CLR_BRIGHT_MAGENTA, '_'),
    SPELL(NULL, "paperback",    0, 0, 0, 0, 1, 0, CLR_BRIGHT_BLUE, '_'),
    SPELL(NULL, "stapled",      0, 0, 0, 0, 1, 0, CLR_GRAY, '_'),
    SPELL(NULL, "pocket",       0, 0, 0, 0, 1, 0, CLR_BRIGHT_MAGENTA, '_'),
    SPELL(NULL, "pale",         0, 0, 0, 0, 1, 0, CLR_BRIGHT_GREEN, '_'),
    SPELL(NULL, "maroon",       0, 0, 0, 0, 1, 0, CLR_RED, '_'),
/* blank spellbook must come last because it retains its description */
    SPELL("blank paper", "plain", P_NONE, 18, 0, 0, 0, 0, HI_PAPER, 0),
/* a special, one of a kind, spellbook */
    OBJECT(OBJ("Book of the Dead", "papyrus"),
           BITS(0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, P_NONE, PAPER), 0,
           SPBOOK_CLASS, 0, 0, 20, 10000, 0, 0, 0, 7, 20, HI_PAPER),
#undef SPELL

/* wands ... */
#define WAND(name,typ,prob,cost,mgc,dir,metal,color) OBJECT( \
        OBJ(name,typ), BITS(0,0,1,0,mgc,1,0,0,0,0,dir,P_NONE,metal), 0, \
            WAND_CLASS, prob, 0, 7, cost, 0, 0, 0, 0, 30, color )
    WAND("light", "glass", 95, 100, 1, NODIR, GLASS, HI_GLASS),
    WAND("secret door detection", "balsa",
         50, 150, 1, NODIR, WOOD, HI_WOOD),
    WAND("enlightenment", "crystal", 15, 150, 1, NODIR, GLASS, HI_GLASS),
    WAND("create monster", "maple", 45, 200, 1, NODIR, WOOD, HI_WOOD),
    WAND("wishing", "pine", 5, 500, 1, NODIR, WOOD, HI_WOOD),
    WAND("nothing", "oak", 25, 100, 0, IMMEDIATE, WOOD, HI_WOOD),
    WAND("striking", "ebony", 75, 150, 1, IMMEDIATE, WOOD, HI_WOOD),
    WAND("make invisible", "marble", 45, 150, 1, IMMEDIATE, MINERAL,
         HI_MINERAL),
    WAND("slow monster", "tin", 50, 150, 1, IMMEDIATE, METAL, HI_METAL),
    WAND("speed monster", "brass", 50, 150, 1, IMMEDIATE, COPPER, HI_COPPER),
    WAND("undead turning", "copper", 50, 150, 1, IMMEDIATE, COPPER, HI_COPPER),
    WAND("polymorph", "silver", 45, 200, 1, IMMEDIATE, SILVER, HI_SILVER),
    WAND("cancellation", "platinum", 45, 200, 1, IMMEDIATE, PLATINUM,
         CLR_WHITE),
    WAND("teleportation", "iridium", 45, 200, 1, IMMEDIATE, METAL,
         CLR_BRIGHT_CYAN),
    WAND("opening", "zinc", 25, 150, 1, IMMEDIATE, METAL, HI_METAL),
    WAND("locking", "aluminum", 25, 150, 1, IMMEDIATE, METAL, HI_METAL),
    WAND("probing", "uranium", 30, 150, 1, IMMEDIATE, METAL, HI_METAL),
    WAND("digging", "iron", 55, 150, 1, RAY, IRON, HI_METAL),
    WAND("magic missile", "steel", 50, 150, 1, RAY, IRON, HI_METAL),
    WAND("fire", "hexagonal", 40, 175, 1, RAY, IRON, HI_METAL),
    WAND("cold", "short", 40, 175, 1, RAY, IRON, HI_METAL),
    WAND("sleep", "runed", 50, 175, 1, RAY, IRON, HI_METAL),
    WAND("death", "long", 5, 500, 1, RAY, IRON, HI_METAL),
    WAND("lightning", "curved", 40, 175, 1, RAY, IRON, HI_METAL),
    WAND(NULL, "forked", 0, 150, 1, 0, WOOD, HI_WOOD),
    WAND(NULL, "spiked", 0, 150, 1, 0, IRON, HI_METAL),
    WAND(NULL, "jeweled", 0, 150, 1, 0, IRON, HI_MINERAL),
#undef WAND

/* coins ... - so far, gold is all there is */
#define COIN(name,prob,metal,worth) OBJECT( \
        OBJ(name,NULL), BITS(0,1,0,0,0,0,0,0,0,0,0,P_NONE,metal), 0, \
            COIN_CLASS, prob, 0, 1, worth, 0, 0, 0, 0, 0, HI_GOLD )
    COIN("gold piece", 1000, GOLD, 1),
#undef COIN

/* gems ... - includes stones and rocks but not boulders */
#define GEM(name,desc,prob,wt,gval,nutr,mohs,glass,color) OBJECT( \
        OBJ(name,desc), \
            BITS(0,1,0,0,0,0,0,0,0,HARDGEM(mohs),0,-P_SLING,glass), 0, \
            GEM_CLASS, prob, 0, 1, gval, 3, 3, 0, 0, nutr, color )
#define ROCK(name,desc,kn,prob,wt,gval,sdam,ldam,mgc,nutr,mohs,glass,color) \
        OBJECT( OBJ(name,desc), \
            BITS(kn,1,0,0,mgc,0,0,0,0,HARDGEM(mohs),0,-P_SLING,glass), 0, \
            GEM_CLASS, prob, 0, wt, gval, sdam, ldam, 0, 0, nutr, color )
    /* makedefs and mkobj assume that all true gems come first */
    GEM("dilithium crystal", "white", 2, 1, 4500, 15, 5, GEMSTONE, CLR_WHITE),
    GEM("diamond", "white", 3, 1, 4000, 15, 10, GEMSTONE, CLR_WHITE),
    GEM("ruby", "red", 4, 1, 3500, 15, 9, GEMSTONE, CLR_RED),
    GEM("jacinth", "orange", 3, 1, 3250, 15, 9, GEMSTONE, CLR_ORANGE),
    GEM("sapphire", "blue", 4, 1, 3000, 15, 9, GEMSTONE, CLR_BLUE),
    GEM("black opal", "black", 3, 1, 2500, 15, 8, GEMSTONE, CLR_BLACK),
    GEM("emerald", "green", 5, 1, 2500, 15, 8, GEMSTONE, CLR_GREEN),
    GEM("turquoise", "green", 6, 1, 2000, 15, 6, GEMSTONE, CLR_GREEN),
    GEM("citrine", "yellow", 4, 1, 1500, 15, 6, GEMSTONE, CLR_YELLOW),
    GEM("aquamarine", "green", 6, 1, 1500, 15, 8, GEMSTONE, CLR_GREEN),
    GEM("amber", "yellowish brown", 8, 1, 1000, 15, 2, GEMSTONE, CLR_BROWN),
    GEM("topaz", "yellowish brown", 10, 1, 900, 15, 8, GEMSTONE, CLR_BROWN),
    GEM("jet", "black", 6, 1, 850, 15, 7, GEMSTONE, CLR_BLACK),
    GEM("opal", "white", 12, 1, 800, 15, 6, GEMSTONE, CLR_WHITE),
    GEM("chrysoberyl", "yellow", 8, 1, 700, 15, 5, GEMSTONE, CLR_YELLOW),
    GEM("garnet", "red", 12, 1, 700, 15, 7, GEMSTONE, CLR_RED),
    GEM("amethyst", "violet", 14, 1, 600, 15, 7, GEMSTONE, CLR_MAGENTA),
    GEM("jasper", "red", 15, 1, 500, 15, 7, GEMSTONE, CLR_RED),
    GEM("fluorite", "violet", 15, 1, 400, 15, 4, GEMSTONE, CLR_MAGENTA),
    GEM("obsidian", "black", 9, 1, 200, 15, 6, GEMSTONE, CLR_BLACK),
    GEM("agate", "orange", 12, 1, 200, 15, 6, GEMSTONE, CLR_ORANGE),
    GEM("jade", "green", 10, 1, 300, 15, 6, GEMSTONE, CLR_GREEN),
    /* Warning: some code in mthrowu.c assumes there are exactly 9 of these.
       (Search for "LAST_GEM + 9".) */
    GEM("worthless piece of white glass", "white", 77, 1, 0, 6, 5, GLASS,
        CLR_WHITE),
    GEM("worthless piece of blue glass", "blue", 77, 1, 0, 6, 5, GLASS,
        CLR_BLUE),
    GEM("worthless piece of red glass", "red", 77, 1, 0, 6, 5, GLASS, CLR_RED),
    GEM("worthless piece of yellowish brown glass", "yellowish brown",
        77, 1, 0, 6, 5, GLASS, CLR_BROWN),
    GEM("worthless piece of orange glass", "orange", 76, 1, 0, 6, 5, GLASS,
        CLR_ORANGE),
    GEM("worthless piece of yellow glass", "yellow", 77, 1, 0, 6, 5, GLASS,
        CLR_YELLOW),
    GEM("worthless piece of black glass", "black", 76, 1, 0, 6, 5, GLASS,
        CLR_BLACK),
    GEM("worthless piece of green glass", "green", 77, 1, 0, 6, 5, GLASS,
        CLR_GREEN),
    GEM("worthless piece of violet glass", "violet", 77, 1, 0, 6, 5, GLASS,
        CLR_MAGENTA),

/* Placement note: there is a wishable subrange for
 * "gray stones" in the o_ranges[] array in objnam.c
 * that is currently everything between luckstones and flint (inclusive).
 */
    ROCK("luckstone", "gray", 0, 10, 10, 60, 3, 3, 1, 10, 7, MINERAL, CLR_GRAY),
    ROCK("loadstone", "gray", 0, 10, 500, 1, 15, 15, 1, 10, 6, MINERAL, CLR_GRAY),
    ROCK("touchstone", "gray", 0, 8, 10, 45, 3, 3, 1, 10, 6, MINERAL, CLR_GRAY),
    ROCK("flint", "gray", 0, 9, 10, 1, 6, 8, 0, 10, 7, MINERAL, CLR_GRAY),
    ROCK("silver nugget", "shiny", 0, 1, 10, 1, 6, 6, 0, 10, 7, SILVER, HI_SILVER),
    ROCK("rock", NULL, 1, 100, 10, 0, 3, 3, 0, 10, 7, MINERAL, CLR_GRAY),
#undef GEM
#undef ROCK

/* miscellaneous ... */
/* Note: boulders and rocks are not normally created at random; the
 * probabilities only come into effect when you try to polymorph them.
 * Boulders weigh more than MAX_CARR_CAP; statues use corpsenm to take
 * on a specific type and may act as containers (both affect weight).
 */
    OBJECT(OBJ("boulder", NULL),
           BITS(1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, P_NONE, MINERAL), 0,
           ROCK_CLASS, 100, 0, 6000, 0, 20, 20, 0, 0, 2000, HI_MINERAL),
    OBJECT(OBJ("statue", NULL),
           BITS(1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, P_NONE, MINERAL), 0,
           ROCK_CLASS, 900, 0, 2500, 0, 20, 20, 0, 0, 2500, CLR_WHITE),

    OBJECT(OBJ("heavy iron ball", NULL),
           BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHACK, P_NONE, IRON), 0,
           BALL_CLASS, 1000, 0, 480, 10, 25, 25, 0, 0, 200, HI_METAL),
    /* +d4 when "very heavy" */
    OBJECT(OBJ("iron chain", NULL),
           BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHACK, P_NONE, IRON), 0,
           CHAIN_CLASS, 1000, 0, 120, 0, 4, 4, 0, 0, 200, HI_METAL),
    /* +1 both l & s */

    OBJECT(OBJ("blinding venom", "splash of venom"),
           BITS(0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, P_NONE, LIQUID), 0,
           VENOM_CLASS, 500, 0, 1, 0, 0, 0, 0, 0, 0, HI_ORGANIC),
    OBJECT(OBJ("vampire blood", "splash of blood"), /* GFP: Vlad */
           BITS(0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, P_NONE, LIQUID), 0,
           VENOM_CLASS,   0, 0, 1, 1, 9, 9, 40, 0, 0, CLR_RED),
    OBJECT(OBJ("acid venom", "splash of venom"),
           BITS(0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, P_NONE, LIQUID), 0,
           VENOM_CLASS, 500, 0, 1, 0, 6, 6, 0, 0, 0, HI_ORGANIC),
    /* +d6 small or large */

/* fencepost, the deadly Array Terminator -- name [1st arg] *must* be NULL */
    OBJECT(OBJ(NULL, NULL), BITS(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, 0), 0,
           ILLOBJ_CLASS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
};      /* objects[] */

#ifndef OBJECTS_PASS_2_

/* perform recursive compilation for second structure */
# undef OBJ
# undef OBJECT
# undef OBJECT2
# define OBJECTS_PASS_2_
# include "objects.c"

#else

void
init_objlist(void)
{
    objects = malloc(sizeof (const_objects));
    memcpy(objects, const_objects, sizeof (const_objects));
}

#endif /* !OBJECTS_PASS_2_ */

/*objects.c*/

