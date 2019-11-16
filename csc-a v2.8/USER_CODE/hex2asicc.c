/*****************************·¢ËÍVDR×Óº¯Êý**********************************/
uint8 HEX2ASICC(uint8 vdr_dat)
{
	uint8 vdr_asicc = 0;
	
	if(vdr_dat<=9)
	{
		vdr_asicc = vdr_dat + 48;
	}
	else if((vdr_dat>=10) && (vdr_dat<=15))
	{
		vdr_asicc = vdr_dat + 55;
	}
	
	return vdr_asicc;
}