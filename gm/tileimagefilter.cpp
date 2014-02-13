/*
 * Copyright 2013 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "gm.h"
#include "SkTileImageFilter.h"
#include "SkBitmapSource.h"

#define WIDTH 400
#define HEIGHT 100
#define MARGIN 12

namespace skiagm {

class TileImageFilterGM : public GM {
public:
    TileImageFilterGM() : fInitialized(false) {
        this->setBGColor(0xFF000000);
    }

protected:
    virtual SkString onShortName() {
        return SkString("tileimagefilter");
    }

    void make_bitmap() {
        fBitmap.allocN32Pixels(80, 80);
        SkCanvas canvas(fBitmap);
        canvas.clear(0xFF000000);
        SkPaint paint;
        paint.setAntiAlias(true);
        paint.setColor(0xD000D000);
        paint.setTextSize(SkIntToScalar(96));
        const char* str = "e";
        canvas.drawText(str, strlen(str), SkIntToScalar(15), SkIntToScalar(65), paint);
    }

    void make_checkerboard() {
        fCheckerboard.allocN32Pixels(80, 80);
        SkCanvas canvas(fCheckerboard);
        canvas.clear(0x00000000);
        SkPaint darkPaint;
        darkPaint.setColor(0xFF404040);
        SkPaint lightPaint;
        lightPaint.setColor(0xFFA0A0A0);
        for (int y = 0; y < 80; y += 16) {
          for (int x = 0; x < 80; x += 16) {
            canvas.save();
            canvas.translate(SkIntToScalar(x), SkIntToScalar(y));
            canvas.drawRect(SkRect::MakeXYWH(0, 0, 8, 8), darkPaint);
            canvas.drawRect(SkRect::MakeXYWH(8, 0, 8, 8), lightPaint);
            canvas.drawRect(SkRect::MakeXYWH(0, 8, 8, 8), lightPaint);
            canvas.drawRect(SkRect::MakeXYWH(8, 8, 8, 8), darkPaint);
            canvas.restore();
          }
        }
    }

    virtual SkISize onISize() {
        return make_isize(WIDTH, HEIGHT);
    }

    virtual void onDraw(SkCanvas* canvas) {
        if (!fInitialized) {
            make_bitmap();
            make_checkerboard();
            fInitialized = true;
        }
        canvas->clear(0x00000000);
        SkPaint paint;

        int x = 0, y = 0;
        for (size_t i = 0; i < 4; i++) {
            SkBitmap* bitmap = (i & 0x01) ? &fCheckerboard : &fBitmap;
            SkRect srcRect = SkRect::MakeXYWH(SkIntToScalar(bitmap->width()/4),
                                              SkIntToScalar(bitmap->height()/4),
                                              SkIntToScalar(bitmap->width()/(i+1)),
                                              SkIntToScalar(bitmap->height()/(i+1)));
            SkRect dstRect = SkRect::MakeXYWH(SkIntToScalar(i * 8),
                                              SkIntToScalar(i * 4),
                                              SkIntToScalar(bitmap->width() - i * 12),
                                              SkIntToScalar(bitmap->height()) - i * 12);
            SkAutoTUnref<SkImageFilter> tileInput(SkNEW_ARGS(SkBitmapSource, (*bitmap)));
            SkAutoTUnref<SkImageFilter> filter(SkNEW_ARGS(
                SkTileImageFilter, (srcRect, dstRect, tileInput)));
            canvas->save();
            canvas->translate(SkIntToScalar(x), SkIntToScalar(y));
            paint.setImageFilter(filter);
            canvas->drawBitmap(fBitmap, 0, 0, &paint);
            canvas->restore();
            x += bitmap->width() + MARGIN;
            if (x + bitmap->width() > WIDTH) {
                x = 0;
                y += bitmap->height() + MARGIN;
            }
        }
    }
private:
    typedef GM INHERITED;
    SkBitmap fBitmap, fCheckerboard;
    bool fInitialized;
};

//////////////////////////////////////////////////////////////////////////////

static GM* MyFactory(void*) { return new TileImageFilterGM; }
static GMRegistry reg(MyFactory);

}
