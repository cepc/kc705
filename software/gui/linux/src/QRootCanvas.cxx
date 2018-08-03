#include "QRootCanvas.hh"

//------------------------------------------------------------------------------

//______________________________________________________________________________
QRootCanvas::QRootCanvas(QWidget* parent)
    : QWidget(parent, 0)
    , fCanvas(0)
{
  // QRootCanvas constructor.

  // set options needed to properly update the canvas when resizing the widget
  // and to properly handle context menus and mouse move events
  setAttribute(Qt::WA_PaintOnScreen, true);
  setAttribute(Qt::WA_OpaquePaintEvent, true);
  setMinimumSize(300, 200);
  setUpdatesEnabled(kFALSE);
  setMouseTracking(kTRUE);

  // register the QWidget in TVirtualX, giving its native window id
  int wid = gVirtualX->AddWindow((ULong_t)winId(), 600, 400);
  // create the ROOT TCanvas, giving as argument the QWidget registered id
  fCanvas = new TCanvas("Root Canvas", width(), height(), wid);
}

//______________________________________________________________________________
void QRootCanvas::mouseMoveEvent(QMouseEvent* e)
{
  // Handle mouse move events.

  if (fCanvas) {
    if (e->buttons() & Qt::LeftButton) {
      fCanvas->HandleInput(kButton1Motion, e->x(), e->y());
    } else if (e->buttons() & Qt::MidButton) {
      fCanvas->HandleInput(kButton2Motion, e->x(), e->y());
    } else if (e->buttons() & Qt::RightButton) {
      fCanvas->HandleInput(kButton3Motion, e->x(), e->y());
    } else {
      fCanvas->HandleInput(kMouseMotion, e->x(), e->y());
    }
  }
}

//______________________________________________________________________________
void QRootCanvas::mousePressEvent(QMouseEvent* e)
{
  // Handle mouse button press events.

  if (fCanvas) {
    switch (e->button()) {
    case Qt::LeftButton:
      fCanvas->HandleInput(kButton1Down, e->x(), e->y());
      break;
    case Qt::MidButton:
      fCanvas->HandleInput(kButton2Down, e->x(), e->y());
      break;
    case Qt::RightButton:
      // does not work properly on Linux...
      // ...adding setAttribute(Qt::WA_PaintOnScreen, true)
      // seems to cure the problem
      fCanvas->HandleInput(kButton3Down, e->x(), e->y());
      break;
    default:
      break;
    }
  }
}

//______________________________________________________________________________
void QRootCanvas::mouseReleaseEvent(QMouseEvent* e)
{
  // Handle mouse button release events.

  if (fCanvas) {
    switch (e->button()) {
    case Qt::LeftButton:
      fCanvas->HandleInput(kButton1Up, e->x(), e->y());
      break;
    case Qt::MidButton:
      fCanvas->HandleInput(kButton2Up, e->x(), e->y());
      break;
    case Qt::RightButton:
      // does not work properly on Linux...
      // ...adding setAttribute(Qt::WA_PaintOnScreen, true)
      // seems to cure the problem
      fCanvas->HandleInput(kButton3Up, e->x(), e->y());
      break;
    default:
      break;
    }
  }
}

//______________________________________________________________________________
void QRootCanvas::resizeEvent(QResizeEvent*)
{
  // Handle resize events.

  if (fCanvas) {
    fCanvas->Resize();
    fCanvas->Update();
  }
}

//______________________________________________________________________________
void QRootCanvas::paintEvent(QPaintEvent*)
{
  // Handle paint events.

  if (fCanvas) {
    fCanvas->Resize();
    fCanvas->Update();
  }
}
