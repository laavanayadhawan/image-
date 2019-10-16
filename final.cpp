
#include <iostream>
#include <fstream>
using namespace std;

 //typedef unsigned short Tuint16;
 //typedef unsigned int Tuint32;
 //typedef int Tint32;

 typedef unsigned short Tuint16;
 typedef unsigned int Tuint32;
 typedef int Tint32;


// typedef short Tint16;
// typedef char Tint8;
// typedef unsigned char Tuint8;

#pragma pack(push,1)
struct BMPFileHeader {
    Tuint16  file_type;          // File type always BM which is 0x4D42 (stored as hex Tuint16  in little endian)
    Tuint32 file_size;               // Size of the file (in bytes)
    Tuint16  reserved1;               // Reserved, always 0
    Tuint16  reserved2;               // Reserved, always 0
    Tuint32 offset_data;             // Start position of pixel data (bytes from the beginning of the file)

    BMPFileHeader()
    {
	file_type = 0x4D42;          // File type always BM which is 0x4D42 (stored as hex Tuint16  in little endian)
	file_size = 0;               // Size of the file (in bytes)
	reserved1 = 0;               // Reserved, always 0
	reserved2 = 0;               // Reserved, always 0
	offset_data = 0;             // Start position of pixel data (bytes from the beginning of the file)

    }

};

struct BMPInfoHeader {
    Tuint32 size;                      // Size of this header (in bytes)
    Tint32 width;                      // width of bitmap in pixels
    Tint32 height;                     // width of bitmap in pixels
					     //       (if positive, bottom-up, with origin in lower left corner)
					     //       (if negative, top-down, with origin in upper left corner)
    Tuint16  planes;                    // No. of planes for the target device, this is always 1
    Tuint16  bit_count;                 // No. of bits per pixel
    Tuint32 compression;               // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
    Tuint32 size_image;                // 0 - for uncompressed images
    Tint32 x_pixels_per_meter;
    Tint32 y_pixels_per_meter;
    Tuint32 colors_used;               // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
    Tuint32 colors_important;          // No. of colors used for displaying the bitmap. If 0 all colors are required

    BMPInfoHeader()
    {
	size = 0;                      // Size of this header (in bytes)
	width = 0;                      // width of bitmap in pixels
	height = 0;                     // width of bitmap in pixels
						 //       (if positive, bottom-up, with origin in lower left corner)
						 //       (if negative, top-down, with origin in upper left corner)
	planes = 1;                    // No. of planes for the target device, this is always 1
	bit_count = 0;                 // No. of bits per pixel
	compression = 0;               // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
	size_image = 0;                // 0 - for uncompressed images
	x_pixels_per_meter = 0;
	y_pixels_per_meter = 0;
	colors_used = 0;               // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
	colors_important =0;          // No. of colors used for displaying the bitmap. If 0 all colors are required

    }

};


#pragma pack(pop)

class BMP {
	private:
    Tuint32 row_stride;

    void write_headers(ofstream &of)
    {
	of.write((const char*)&file_header, sizeof(file_header));
	of.write((const char*)&bmp_info_header, sizeof(bmp_info_header));
	if(bmp_info_header.bit_count == 32)
	{
	    cout << "only 24 bit color supported" <<endl;
	    //of.write((const char*)&bmp_color_header, sizeof(bmp_color_header));
	}
    }

    void write_headers_and_data(ofstream &of)
    {
	write_headers(of);
	of.write((const char*)data, data_size);
    }

    void dump_data(ofstream &of)
    {
    	of.write((const char*)data, data_size);
    }

    // Add 1 to the row_stride until it is divisible with align_stride
    Tuint32 make_stride_aligned(Tuint32 align_stride)
    {
	Tuint32 new_stride = row_stride;
	while
	 (new_stride % align_stride != 0)
	{
	    new_stride++;
	}
	return new_stride;
    }
public:
    BMPFileHeader file_header;
    BMPInfoHeader bmp_info_header;

    unsigned char* data;
    Tuint32 data_size;

    BMP(const char *fname)
    {
	data = NULL;
	row_stride = 0;
	data_size =0;
	read(fname);
    }

   /* BMP(int width, int height) {

	data = NULL;
	row_stride = 0;
	data_size =0;
	if (width <= 0 || height <= 0) {
	    cout << "The image width and height must be positive numbers." << endl;
	    return;
	}

	bmp_info_header.width = width;
	bmp_info_header.height = height;


	bmp_info_header.size = sizeof(BMPInfoHeader);
	file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

	bmp_info_header.bit_count = 24;
	bmp_info_header.compression = 0;
	row_stride = width * 3;
	data_size = row_stride * height;
	data = new unsigned char [data_size];

	Tuint32 new_stride = make_stride_aligned(4);
	file_header.file_size = file_header.offset_data + data_size + bmp_info_header.height * (new_stride - row_stride);
	return;
    }*/


    ~BMP()
    {
	delete []data;
    }

    void DisplayHeaders()
    {
	cout << "width: " << bmp_info_header.width<<endl;
	cout << "height: " << bmp_info_header.height<<endl;
	return;
    }

    void read(const char *fname) {
	//cout<< "In Read: " <<endl;
	ifstream inpf( fname, ios::binary );
	if (inpf)
	{
	    //cout << "file exists: " <<endl;
	    inpf.read((char*)&file_header, sizeof(file_header));
	    if(file_header.file_type != 0x4D42)
	    {
		cout << "Error! Unrecognized file format." <<endl;
		return;
	    }
	    inpf.read((char*)&bmp_info_header, sizeof(bmp_info_header));
	    cout <<"width:" << bmp_info_header.width <<endl;
	    cout <<"height:" << bmp_info_header.height <<endl;

	    // The BMPColorHeader is used only for transparent images
	    if(bmp_info_header.bit_count == 32)
	    {
		cout << " only 24 bit images supported" <<endl;
		return;
	    }
	    //cout <<"offset: "<<file_header.offset_data <<endl;
	    // Jump to the pixel data location
	    inpf.seekg(file_header.offset_data, ios::beg);

	    // Adjust the header fields for output.
	    // Some editors will put extra info in the image file, we only save the headers and the data.
	    bmp_info_header.size = sizeof(BMPInfoHeader);
	    file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

	    file_header.file_size = file_header.offset_data;

	    if (bmp_info_header.height < 0)
	    {
		cout << "The program can treat only BMP images with the origin in the bottom left corner!" << endl;
		return;
	    }

	    data_size = bmp_info_header.width * bmp_info_header.height * bmp_info_header.bit_count / 8;
	    data = new unsigned char [data_size];

	    // Here we check if we need to take into account row padding
	    if (bmp_info_header.width % 4 == 0)
	    {
		//cout << "data size " << data_size <<endl;
		inpf.read((char*)data, data_size);
		file_header.file_size += data_size;
	    }
	    else
	    {
		row_stride = bmp_info_header.width * bmp_info_header.bit_count / 8;
		Tuint32 new_stride = make_stride_aligned(4);
		Tuint32 padding_row_size = new_stride - row_stride;
		unsigned char* buffer = new unsigned char [padding_row_size];
		//cout <<"bmp file needs aligned padding on width" <<endl;
		for
		 (int y = 0; y < bmp_info_header.height; y++)
		{
		    inpf.read((char*)(data + row_stride * y), row_stride);
		    inpf.read((char*)buffer, padding_row_size);
		    //cout <<"row num "<< y <<endl;

		}
		//cout << "done all rows" << endl;
		delete []buffer;
		//cout <<"delete buffer" <<endl;

		file_header.file_size += data_size + bmp_info_header.height * padding_row_size;
		//cout <<"done reading all rows " << endl;
	    }
	}
	else
	{
	    cout << "Unable to open the input image file." <<endl;
	}
		ofstream of("dd.raw",ios::binary);
		of.write((const char*)data,data_size);
		of.close();

    }

    void write(const char *fname)
    {
	ofstream of( fname, ios::binary );
	if (of) {
	    if (bmp_info_header.bit_count == 32) {
		cout <<"Only 24 bit images are handled"<<endl;
		return;
	    }
	    else if (bmp_info_header.bit_count == 24)
	    {
		if (bmp_info_header.width % 4 == 0)
		{
		    write_headers_and_data(of);
		}
		else
		{
		    Tuint32 new_stride = make_stride_aligned(4);
		    Tuint32 padding_row_size = new_stride - row_stride;
		    unsigned char * padding_row = new unsigned char [padding_row_size];

		    write_headers(of);

		    for (int y = 0; y < bmp_info_header.height; ++y)
		    {
			of.write((const char*)(data + row_stride * y), row_stride);
			of.write((const char*)padding_row, padding_row_size);
		    }
		    delete []padding_row;
		}
	    }
	    else
	    {
		cout << "The program can treat only 24 or 32 bits per pixel BMP files"<< endl;
	    }
	}
	else
	{
	    cout << "Unable to open the output image file."<<endl;
	}
    }

    void fill_region(Tuint32 x0, Tuint32 y0, Tuint32 w, Tuint32 h, unsigned char B, unsigned char G, unsigned char R)
    {
	if (x0 + w > (Tuint32)bmp_info_header.width || y0 + h > (Tuint32)bmp_info_header.height)
	{
	    cout << "The region does not fit in the image!" <<endl;
	}

	Tuint32 channels = bmp_info_header.bit_count / 8;
	for (Tuint32 y = y0; y < y0 + h; ++y)
	{
	    for (Tuint32 x = x0; x < x0 + w; ++x)
	    {
		data[channels * (y * bmp_info_header.width + x) + 0] = B;
		data[channels * (y * bmp_info_header.width + x) + 1] = G;
		data[channels * (y * bmp_info_header.width + x) + 2] = R;

	    }
	}
    }

    void draw_rectangle(Tuint32 x0, Tuint32 y0, Tuint32 w, Tuint32 h,
			unsigned char B, unsigned char G, unsigned char R,  unsigned char line_w)
    {
	if (x0 + w > (Tuint32)bmp_info_header.width || y0 + h > (Tuint32)bmp_info_header.height)
	{
	    cout << "The rectangle does not fit in the image!" <<endl;
	}

	fill_region(x0, y0, w, line_w, B, G, R);                                             // top line
	fill_region(x0, (y0 + h - line_w), w, line_w, B, G, R);                              // bottom line
	fill_region((x0 + w - line_w), (y0 + line_w), line_w, (h - (2 * line_w)), B, G, R);  // right line
	fill_region(x0, (y0 + line_w), line_w, (h - (2 * line_w)), B, G, R);                 // left line
    }
    void rotate( int ang )
    {
    	if((ang==0)||(ang==360))
	    {
	    	return;
	    }
    	unsigned char* data_rot = new unsigned char[data_size];

    	Tuint32 channels = bmp_info_header.bit_count / 8;
    	for(int y=0;y<(bmp_info_header.height);y++)
    	{
    		for(int x=0;x<(bmp_info_header.width);x++)
    		{

	    		if(ang==90)
	    		{
	    			data_rot[channels * (x * bmp_info_header.height + y) + 0] = data[channels * (y* bmp_info_header.width + bmp_info_header.width-x-1)  + 0];
	    			data_rot[channels * (x * bmp_info_header.height + y) + 1] = data[channels * (y* bmp_info_header.width + bmp_info_header.width-x-1)  + 1];
	    			data_rot[channels * (x * bmp_info_header.height + y) + 2] = data[channels * (y* bmp_info_header.width + bmp_info_header.width-x-1)  + 2];
	    		}
	    		else if(ang==180)
	    		{
	    			data_rot[channels * (y* bmp_info_header.width + x)  + 0] = data[channels * ((bmp_info_header.height-y-1)*bmp_info_header.width + bmp_info_header.width-x-1)  + 0];
	    			data_rot[channels * (y * bmp_info_header.width + x) + 1] = data[channels * ((bmp_info_header.height-y-1)* bmp_info_header.width + bmp_info_header.width-x-1)  + 1];
	    			data_rot[channels * (y * bmp_info_header.width + x) + 2] = data[channels * ((bmp_info_header.height-y-1)* bmp_info_header.width + bmp_info_header.width-x-1)  + 2];  			
	    		}
	    		else if( ang==270 )
	    		{
	    			data_rot[channels * (x* bmp_info_header.height + y)  + 0] = data[channels * ((bmp_info_header.height-y-1)* bmp_info_header.width + x)  + 0];
	    			data_rot[channels * (x * bmp_info_header.height + y) + 1] = data[channels * ((bmp_info_header.height-y-1)* bmp_info_header.width + x)  + 1];
	    			data_rot[channels * (x * bmp_info_header.height + y) + 2] = data[channels * ((bmp_info_header.height-y-1)* bmp_info_header.width + x)  + 2]; 			
	    		}
	    		else
	    		{
	    			cout<<" enter a multiple of 90 degrees ."<<endl;
	    		}


    		}
    	}
    	delete data;
    	data = data_rot;
    	if ( (ang == 90) || (ang == 270))
    	{
	    	int t = bmp_info_header.width;
	    	bmp_info_header.width = bmp_info_header.height;
	    	bmp_info_header.height = t;
	    }
    }
    void water_reflection()
    {
    	Tuint32 channels = bmp_info_header.bit_count / 8;
    	unsigned char* data_ref = new unsigned char [channels*bmp_info_header.width*bmp_info_header.height];
    	for(int y=0;y<(bmp_info_header.height);y++)
    	{
    		for(int x=0;x<(bmp_info_header.width);x++)
    		{
    			data_ref[channels * (y* bmp_info_header.width + x)  + 0] = data[channels * ((bmp_info_header.height-y-1)* bmp_info_header.width + x)  + 0];
	    		data_ref[channels * (y * bmp_info_header.width + x) + 1] = data[channels * ((bmp_info_header.height-y-1)* bmp_info_header.width + x)  + 1];
	    		data_ref[channels * (y * bmp_info_header.width + x) + 2] = data[channels * ((bmp_info_header.height-y-1)* bmp_info_header.width + x)  + 2]; 
    		}
    	}
    	delete data;
    	data = data_ref;
    }
    void mirror_reflection()
    {

    	Tuint32 channels = bmp_info_header.bit_count / 8;
    	unsigned char* data_ref = new unsigned char [channels*bmp_info_header.width*bmp_info_header.height];
    	  
		for(int y=0;y<(bmp_info_header.height);y++)
    	{
    		for(int x=0;x<(bmp_info_header.width);x++)
    		{
    			data_ref[channels * (y* bmp_info_header.width + x)  + 0] = data[channels * (y* bmp_info_header.width + bmp_info_header.width-x-1)  + 0];
	    		data_ref[channels * (y * bmp_info_header.width + x) + 1] = data[channels * (y* bmp_info_header.width + bmp_info_header.width-x-1)  + 1];
	    		data_ref[channels * (y * bmp_info_header.width + x) + 2] = data[channels * (y* bmp_info_header.width + bmp_info_header.width-x-1)  + 2]; 
    		}
    	}
    	delete data;
    	data = data_ref;
    }
    void reduction(int n)
    {
    	Tuint32 channels = bmp_info_header.bit_count / 8;
    	int red_width = bmp_info_header.width/n;
    	int red_height = bmp_info_header.height/n;
    	int red_data_size = channels*red_height*red_width;
    	
    	unsigned char* data_red = new unsigned char[red_data_size];
    	
		for(int y=0;y<red_height;y++)
    	{
    		for(int x=0;x<red_width;x++)
    		{
    			float mean0=0;
    			float mean1=0;
    			float mean2=0;
				for(int k=0;k<n;k++)
				{
					
					if((n*y+k) >= bmp_info_header.height)
						break;

					for(int l=0;l<n;l++)
					{
						if((n*x+l) >= bmp_info_header.width)
							break;

						mean0 += data[channels * ((n*y+k)* bmp_info_header.width + n*x+l)  + 0];
						

						mean1 += data[channels * ((n*y+k)* bmp_info_header.width + n*x+l)  + 1];
						
						mean2 += data[channels * ((n*y+k)* bmp_info_header.width + n*x+l)  + 2];
						

					}
				}
				

				mean0=float((mean0)/(n*n));
				mean1=float((mean1)/(n*n));
				mean2=float((mean2)/(n*n)); 
				/*mean0 = data[channels * ((n*y + n/2)* bmp_info_header.width + n*x+n/2)  + 0];
				mean1 = data[channels * ((n*y + n/2)* bmp_info_header.width + n*x+n/2)  + 1];
				mean2 = data[channels * ((n*y + n/2)* bmp_info_header.width + n*x+n/2)  + 2];*/

				if(channels * (y* red_width + x) > (red_data_size - 3))
				{
					cout <<"problem";
					break;
				}
    			data_red[channels * (y* red_width + x)  + 0]= mean0;
    			data_red[channels * (y * red_width + x) + 1]= mean1;
    			data_red[channels * (y * red_width + x) + 2]= mean2;
    			
    		}
    		cout<<"y = "<< y <<endl;

    	
    	}
    	

    	delete []data;
    	data = data_red;
    	data_size = red_data_size;

	    bmp_info_header.width = red_width;
	    bmp_info_header.height = red_height;
	    row_stride = bmp_info_header.width * bmp_info_header.bit_count / 8;
	    file_header.file_size = sizeof(BMPInfoHeader)+
	    							sizeof(BMPFileHeader)+
	    							bmp_info_header.width*bmp_info_header.height*channels;

	    cout <<"done reduction" <<endl;
    }

    void enlarge(int n)
    {
    	Tuint32 channels = bmp_info_header.bit_count / 8;
    	int enl_width = bmp_info_header.width*n;
    	int enl_height = bmp_info_header.height*n;
    	int enl_data_size = channels*enl_height*enl_width;
    	unsigned char* data_enl = new unsigned char[enl_data_size];

		for(int y=0;y<(bmp_info_header.height);y++)
    	{
    		for(int x=0;x<(bmp_info_header.width);x++)
    		{
    			for(int k=0;k<n;k++)
				{
					for(int l=0;l<n;l++)
					{
    					data_enl[channels * ((n*y+k)* enl_width + n*x+l)  + 0] = data[channels * (y* bmp_info_header.width + x)  + 0];
    					data_enl[channels * ((n*y+k)* enl_width + n*x+l)  + 1] = data[channels * (y * bmp_info_header.width + x) + 1];
    					data_enl[channels * ((n*y+k)* enl_width + n*x+l)  + 2] = data[channels * (y * bmp_info_header.width + x) + 2];
    				}
    			}
    		}
    	}

    	delete []data;
    	data = data_enl;
    	data_size = enl_data_size;
    	bmp_info_header.width = enl_width;
	    bmp_info_header.height = enl_height;
	    row_stride = bmp_info_header.width * bmp_info_header.bit_count / 8;
	    file_header.file_size = sizeof(BMPInfoHeader)+
	    							sizeof(BMPFileHeader)+
	    							bmp_info_header.width*bmp_info_header.height*channels;
    }

    void brightness(float val)
    {
    	Tuint32 channels = bmp_info_header.bit_count / 8;
    	float brt_val;
		for(int y=0;y<(bmp_info_header.height);y++)
    	{
    		for(int x=0;x<(bmp_info_header.width);x++)
    		{
    			brt_val = data[channels * (y* bmp_info_header.width + x)  + 0] * val;
    			if(int(brt_val) >= 255)
    			{
    				data[channels * (y* bmp_info_header.width + x)  + 0] = 255;
    			}
    			else
    			{
    				data[channels * (y* bmp_info_header.width + x)  + 0] = (int) brt_val;
    			}
    			brt_val = data[channels * (y* bmp_info_header.width + x)  + 1] * val;
    			if(int(brt_val) >= 255)
    			{
    				data[channels * (y* bmp_info_header.width + x)  + 1] = 255;
    			}
    			else
    			{
    				data[channels * (y* bmp_info_header.width + x)  + 1] = (int) brt_val;
    			}
    			brt_val = data[channels * (y* bmp_info_header.width + x)  + 2] * val;
    			if(int(brt_val) >= 255)
    			{
    				data[channels * (y* bmp_info_header.width + x)  + 2] = 255;
    			}
    			else
    			{
    				data[channels * (y* bmp_info_header.width + x)  + 2] = (int) brt_val;
    			}
    		}
    	}
    }
    void crop(int beg_x, int beg_y, int end_x, int end_y)
    {
    	Tuint32 channels = bmp_info_header.bit_count / 8;
    	int crp_w= end_x - beg_x;
    	int crp_h= end_y - beg_y;
    	int crp_size= channels*crp_w*crp_h;
    	unsigned char* data_crp= new unsigned char[crp_size];
		for(int y=beg_y;y<end_y;y++)
		{
			for(int x=beg_x;x<end_x;x++)
			{
				data_crp[channels * ((y-beg_y)* crp_w + x-beg_x)  + 0] = data[channels * (y* bmp_info_header.width + x)  + 0];
				data_crp[channels * ((y-beg_y)* crp_w + x-beg_x)  + 1] = data[channels * (y* bmp_info_header.width + x)  + 1];
				data_crp[channels * ((y-beg_y)* crp_w + x-beg_x)  + 2] = data[channels * (y* bmp_info_header.width + x)  + 2];
			}
		}
		delete []data;
    	data = data_crp;
    	data_size = crp_size;

	    bmp_info_header.width = crp_w;
	    bmp_info_header.height = crp_h;
	    row_stride = bmp_info_header.width * bmp_info_header.bit_count / 8;
	    file_header.file_size = sizeof(BMPInfoHeader)+
	    							sizeof(BMPFileHeader)+
	    							bmp_info_header.width*bmp_info_header.height*channels;
    }
    void superimpose(int beg_x, int beg_y, int end_x, int end_y, unsigned char* data_sp)
    {
    	Tuint32 channels = bmp_info_header.bit_count / 8;
    	int sp_w=end_x-beg_x;
		for(int y=beg_y;y<end_y;y++)
		{
			for(int x=beg_x;x<end_x;x++)
			{
				data[channels * (y* bmp_info_header.width + x)  + 0] = data_sp[channels * ((y-beg_y)* sp_w + x-beg_x)  + 0];
				data[channels * (y* bmp_info_header.width + x)  + 1] = data_sp[channels * ((y-beg_y)* sp_w + x-beg_x)  + 1];
				data[channels * (y* bmp_info_header.width + x)  + 2] = data_sp[channels * ((y-beg_y)* sp_w + x-beg_x)  + 2];
			}
		}
    }

};
void showChoices()
{
	cout << "                       ~MENU~                  " << endl;
	cout << "1: Fill region " << endl;
	cout << "2: Rotate " << endl;
	cout << "3: Mirror Reflection " << endl;
	cout << "4: Water Reflection " << endl;
	cout << "5: Enlarge "<< endl;
	cout << "6: Brightness "<<endl;
	cout << "7: Reduction " <<endl;
	cout << "8: Crop "<< endl;
	cout << "9: Superimpose "<<endl;

	cout << "10: Exit " << endl;
	cout << "Enter your choice :";
}

int main()
{
    

    cout << "size of file header: " << sizeof(BMPFileHeader) <<endl;
    cout << "size of info header: " << sizeof(BMPInfoHeader) <<endl;
    getchar();
    BMP bmptest("penguin (1).bmp");
    bmptest.DisplayHeaders();
     int choice;
     do
	{
		showChoices();
		cin >> choice;
		switch (choice)
		{
		case 1:
			{ bmptest.fill_region(50, 50, 100, 100, 60, 12, 75);
			  cout<<"Done fill.";
			  bmptest.write("penguin1_cpy.bmp");
			  break;
			 }
		case 2:
			{ bmptest.rotate(270);
			  bmptest.write("penguin1_cpy.bmp");
			  break;
			}
		case 3:
			{ bmptest.mirror_reflection();
              bmptest.write("penguin1_cpy.bmp");
			  break;
			}
		case 4:
			{ bmptest.water_reflection();
              bmptest.write("penguin1_cpy.bmp");
			  break;
			}
		case 5:
            { bmptest.enlarge(4);
              bmptest.write("penguin1_cpy.bmp");
			break;
		    }
		case 6:
		    { bmptest.brightness(1.5);
		      bmptest.write("penguin1_cpy.bmp");
		      break;
		    }
		case 7:
		    { bmptest.reduction(3);
		      bmptest.write("penguin1_cpy.bmp");
		      break;
		    }
		case 8:
		    { bmptest.crop(10,10,1000,1000);
		      bmptest.write("penguin1_cpy.bmp");
		      break;
		    }
		case 9:
		    { BMP suptest("lily (1).bmp");
             bmptest.superimpose(700,700,987,1001,suptest.data);
              bmptest.write("penguin1_cpy.bmp");
              break;
            }
        case 10:
            {
            	
                break;
            }
		default:
			
				cout << "Invalid input" << endl;
           
		}
	}while (choice != 10);

    
}
