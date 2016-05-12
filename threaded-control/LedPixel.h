#ifndef _LEDPIXEL_H
#define _LEDPIXEL_H


class LedPixel {

  private:
  int id;
  uint32_t color;

  public:

  LedPixel(void);
  LedPixel(int,uint32_t);

  void set_color(uint32_t);
  uint32_t get_color();
  void set_id(int);
  int get_id();

};



LedPixel::LedPixel(void){
  id = color = 0;
}

LedPixel::LedPixel(int id_value, uint32_t color_value){
  id = id_value;
  color = color_value;
}

void LedPixel::set_color(uint32_t value){
  color = value;
}
uint32_t LedPixel::get_color(){
  return color;
}

void LedPixel::set_id(int value){
  id = value;
}

int LedPixel::get_id(){
  return id;
}



#endif
