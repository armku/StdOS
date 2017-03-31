#ifndef __Random_H__
#define __Random_H__

// Ëæ»úÊı
class Random
{
public:
	Random();
	Random(int seed);

	int Next() const;
	int Next(int max) const;
	void Next(Buffer& bs) const;
};

#endif
