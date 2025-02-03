// Extra arg are the y_offset of the image
#define TRAINER_BACK_PIC(trainer, imgPalName, imgSize, y)                                          \
	[TRAINER_BACK_PIC_##trainer] =                                                                 \
	{                                                                                              \
		.pic = { (const u32 *)sTrainerBackPic_##imgPalName, imgSize, TRAINER_BACK_PIC_##trainer }, \
		.palette = { sTrainerPalette_##imgPalName##BackPic, TRAINER_BACK_PIC_##trainer },          \
		.coords = { .size = 8, .y_offset = y },                                                    \
		.anims = sBackAnims_##imgPalName,                                                          \
	}

const struct TrainerPic gTrainerBackPicTable[] =
{
	TRAINER_BACK_PIC(RED, Red, 0x2800, 5),
	TRAINER_BACK_PIC(LEAF, Leaf, 0x2800, 5),
	TRAINER_BACK_PIC(POKEDUDE, Pokedude, 0x2000, 4),
	TRAINER_BACK_PIC(OLD_MAN, OldMan, 0x2000, 4),
	TRAINER_BACK_PIC(RS_BRENDAN, RSBrendan, 0x2000, 4),
	TRAINER_BACK_PIC(RS_MAY, RSMay, 0x2000, 4),
};