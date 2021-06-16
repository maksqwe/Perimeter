// TODO: change encoding to utf-8

#ifndef __SOUND_TRACK_H__
#define __SOUND_TRACK_H__

class SoundTrack
{
public:
	SoundTrack();

	bool empty() const { return fileNames.empty(); }

	const char* fileName() const;

	template<class Archive>	
	void serialize(Archive& ar) {
		ar & WRAP_NAME(fileName_, "fileName");
		ar & TRANSLATE_OBJECT(fileNames, "����� ������");
		ar & TRANSLATE_OBJECT(randomChoice, "��������� �����");
		if(ar.isInput() && fileNames.empty() && !fileName_.empty())
			fileNames.push_back(fileName_);
	}

private:
	vector<string> fileNames; 
	string fileName_; 
	bool randomChoice;
	mutable int index;
};

typedef SoundTrack SoundTracks[3];

#endif //__SOUND_TRACK_H__