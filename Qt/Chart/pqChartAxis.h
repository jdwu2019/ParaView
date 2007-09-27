/*=========================================================================

   Program: ParaView
   Module:    pqChartAxis.h

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.1. 

   See License_v1.1.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

/// \file pqChartAxis.h
/// \date 12/1/2006

#ifndef _pqChartAxis_h
#define _pqChartAxis_h


#include "QtChartExport.h"
#include <QObject>
#include "pqChartPixelScale.h" // Needed for enum

class pqChartAxisModel;
class pqChartAxisOptions;
class pqChartAxisInternal;
class pqChartContentsSpace;
class QPainter;
class QRect;


/// \class pqChartAxis
/// \brief
///   The pqChartAxis class is used to draw an orthogonal chart
///   axis.
///
/// The axis object stores the drawing parameters for an axis as well
/// as a map between pixel and value coordinates. The pixel/value
/// mapping must have the pixel and value ranges set before it is
/// valid. This mapping can be used to determine the layout of the
/// chart data.
///
/// The axis displays the labels provided by the axis model. The axis
/// model can be generated by the axis when the best-fit options is
/// turned on. The axis labels are generated during the layout process
/// using the size of the display area.
///
/// The axis labels can be displayed using a linear or logarithmic
/// scale. The logarithmic scale can only be used if the axis range
/// is greater than zero.
///
/// If the axis has too many labels to display in the provided space,
/// some of the labels will not be drawn. Tick marks will still be
/// drawn for all the axis labels in this case.
class QTCHART_EXPORT pqChartAxis : public QObject
{
  Q_OBJECT

public:
  enum AxisLocation {
    Left,  ///< The axis is on the left of the chart.
    Top,   ///< The axis is on the top of the chart.
    Right, ///< The axis is on the right of the chart.
    Bottom ///< The axis is on the bottom of the chart.
  };

public:
  /// \brief
  ///   Creates a chart axis view.
  /// \param location Where on the chart the axis will be drawn.
  /// \param parent The parent object.
  pqChartAxis(AxisLocation location, QObject *parent=0);
  virtual ~pqChartAxis();

  /// \name Setup Methods
  //@{
  /// \brief
  ///   Gets the chart axis model.
  /// \return
  ///   A pointer to the chart axis model.
  pqChartAxisModel *getModel() const {return this->Model;}

  /// \brief
  ///   Sets the chart axis model.
  /// \param model The model to display.
  void setModel(pqChartAxisModel *model);

  /// \brief
  ///   Sets the neighboring axes if any.
  ///
  /// The neighboring axes are used when laying out the axis. The
  /// axis pixel positions are adjusted to accound for the space
  /// requirements of its neighbors.
  ///
  /// \param atMin The axis at the minimum value end.
  /// \param atMax The axis at the maximum value end.
  void setNeigbors(const pqChartAxis *atMin, const pqChartAxis *atMax);

  /// \brief
  ///   Sets the parallel axis.
  /// \param across The axis parallel to this one.
  void setParallelAxis(const pqChartAxis *across);

  /// \brief
  ///   Sets the axis' contents space object.
  /// \param contents The new contents space object.
  void setContentsScpace(const pqChartContentsSpace *contents);

  /// \brief
  ///   Gets the axis location on the chart.
  /// \return
  ///   The axis location on the chart.
  AxisLocation getLocation() const {return this->Location;}

  /// \brief
  ///   Sets the axis scale type.
  /// \param scale The new axis scale type.
  void setScaleType(pqChartPixelScale::ValueScale scale);

  /// \brief
  ///   Gets whether or not the axis labels are generated from the
  ///   view size.
  /// \return
  ///   True if the axis labels are generated from the view size.
  bool isBestFitGenerated() const;

  /// \brief
  ///   Sets whether or not the axis labels are generated from the
  ///   view size.
  /// \param on True if the axis labels should be generated.
  void setBestFitGenerated(bool on);

  /// \brief
  ///   Sets whether or not data is available for the axis.
  ///
  /// This parameter is used when the best fit range is zero. If data
  /// is available, the axis generates labels around the data value.
  /// This is needed when the data points are the same value for an
  /// axis, such as a vertical or horizontal line on a line chart.
  ///
  /// \param available True if data is available.
  void setDataAvailable(bool available);

  /// \brief
  ///   Gets the value range used when generating the axis labels.
  /// \param min Used to return the minimum value.
  /// \param max Used to return the maximum value.
  void getBestFitRange(pqChartValue &min, pqChartValue &max) const;

  /// \brief
  ///   Sets the value range used when generating the axis labels.
  /// \param min The minimum value.
  /// \param max The maximum value.
  void setBestFitRange(const pqChartValue &min, const pqChartValue &max);

  /// \brief
  ///   Gets whether the maximum will use extra padding.
  /// \return
  ///   True if the maximum will use extra padding.
  /// \sa pqChartAxis::setExtraMaxPadding(bool)
  bool isMaxExtraPadded() const;

  /// \brief
  ///   Sets whether or not extra padding is used for the maximum.
  ///
  /// This setting only affects the best-fit layout. When extra
  /// padding is turned on and the maximum value is a multiple
  /// of the determined interval, an extra interval will be added.
  /// The extra interval padding can be used to make the chart look
  /// more pleasing in some cases.
  ///
  /// \param on True if extra padding should be used for the maximum.
  void setExtraMaxPadding(bool on);

  /// \brief
  ///   Gets whether the minimum will use extra padding.
  /// \return
  ///   True if the minimum will use extra padding.
  /// \sa pqChartAxis::setExtraMaxPadding(bool)
  bool isMinExtraPadded() const;

  /// \brief
  ///   Sets whether or not extra padding is used for the minimum.
  /// \param on True if extra padding should be used for the minimum.
  /// \sa pqChartAxis::setExtraMaxPadding(bool)
  void setExtraMinPadding(bool on);

  /// \brief
  ///   Gets whether or not the space for the axis is too small.
  /// \return
  ///   True if the space for the axis is too small.
  bool isSpaceTooSmall() const;

  /// \brief
  ///   Sets whether or not the space for the axis is too small.
  /// \param tooSmall True if the space for the axis is too small.
  void setSpaceTooSmall(bool tooSmall);
  //@}

  /// \name Drawing Parameters
  //@{
  /// \brief
  ///   Gets the chart axis drawing options.
  /// \return
  ///   A pointer to the chart axis drawing options.
  pqChartAxisOptions *getOptions() const {return this->Options;}

  /// \brief
  ///   Sets the chart axis drawing options.
  ///
  /// This method sets all the axis options at once, which can prevent
  /// unnecessary view updates.
  ///
  /// \param options The new axis drawing options.
  void setOptions(const pqChartAxisOptions &options);
  //@}

  /// \name Display Methods
  //@{
  /// \brief
  ///   Used to layout the chart axis.
  ///
  /// This method must be called before the axis can be drawn.
  ///
  /// \param area The total chart area.
  /// \sa pqChartAxis::drawAxis(QPainter &, const QRect &)
  void layoutAxis(const QRect &area);

  /// \brief
  ///   Used to adjust the bounding width for the axis.
  ///
  /// This method only affects left and right locations. It is called
  /// after the neighboring axes have been layed out. The neighboring
  /// axes may need more space to display the labels.
  void adjustAxisLayout();

  /// \brief
  ///   Gets the space needed for the axis.
  ///
  /// If the axis is horizontal, the space needed is the preferred
  /// height. If the axis is vertical, the space needed is the
  /// preferred width. The preferred height can be obtained at any
  /// time. The preferred width is only valid after a layout.
  ///
  /// \return
  ///   The space needed for the axis.
  int getPreferredSpace() const;

  /// \brief
  ///   Gets the axis label font height.
  /// \return
  ///   The axis label font height.
  int getFontHeight() const;

  /// \brief
  ///   Gets the maximum label width.
  ///
  /// The maximum label width is only valid after a layout.
  ///
  /// \return
  ///   The maximum label width.
  int getMaxLabelWidth() const;

  /// \brief
  ///   Used to draw the chart axis.
  ///
  /// The axis needs to be layed out before it can be drawn.
  /// Separating the layout and drawing functions improves the
  /// repainting performance.
  ///
  /// \param painter The painter to use.
  /// \param area The area that needs to be painted.
  /// \sa pqChartAxis::layoutAxis(const QRect &)
  void drawAxis(QPainter &painter, const QRect &area) const;
  //@}

  /// \name Location Methods
  //@{
  /// \brief
  ///   Gets the bounding rectangle for the axis.
  ///
  /// The bounding box returned is only valid after a layout.
  ///
  /// \param bounds Used to return the bounds.
  void getBounds(QRect &bounds) const;

  /// \brief
  ///   Gets the pixel to value scale for the axis.
  /// \return
  ///   A pointer to the pixel to value scale.
  const pqChartPixelScale *getPixelValueScale() const;

  /// \brief
  ///   Gets wether or not the given label tickmark is visible.
  /// \param index The index of the label.
  /// \return
  ///   True if the given label tickmark is visible.
  bool isLabelTickVisible(int index) const;

  /// \brief
  ///   Gets the location of the given label.
  /// \param index The index of the label.
  /// \return
  ///   The location of the given label.
  int getLabelLocation(int index) const;
  //@}

signals:
  /// Emitted when the axis needs to be layed out again.
  void layoutNeeded();

  /// Emitted when the axis needs to be repainted.
  void repaintNeeded();

  /// \brief
  ///   Emitted when the pixel-value scale changes.
  ///
  /// The pixel-value scale is only modified inside the axis layout
  /// method. Charts using this signal should never emit
  /// \c layoutNeeded when responding to this signal. Instead, set a
  /// flag to use when the chart layout method is called.
  void pixelScaleChanged();

public slots:
  /// Resets the chart axis view.
  void reset();

private slots:
  /// Updates the font height and clears the label width cache.
  void handleFontChange();

  /// Clears the calculated label widths.
  void clearLabelWidthCache();

  /// \brief
  ///   Adds the new label in the given location.
  /// \param index Where to insert the new label.
  void insertLabel(int index);

  /// \brief
  ///   Cleans up the view data for the given index.
  /// \param index The label being removed.
  void startLabelRemoval(int index);

  /// \brief
  ///   Finished the label removal by requesting a relayout.
  /// \param index The label being removed.
  void finishLabelRemoval(int index);

private:
  /// \brief
  ///   Get the estimated maximum label width.
  ///
  /// This method is used when using a best-fit layout. The label
  /// width is estimated using the min and max values.
  ///
  /// \return
  ///   The estimated maximum label width.
  int getLabelWidthGuess() const;

  /// \brief
  ///   Generates labels for a linear, best-fit layout.
  /// \param contents The axis contents area.
  void generateLabels(const QRect &contents);

  /// \brief
  ///   Generates labels for a logarithmic, best-fit layout.
  /// \param contents The axis contents area.
  void generateLogLabels(const QRect &contents);

private:
  pqChartAxisInternal *Internal;    ///< Stores the view data.
  pqChartAxisOptions *Options;      ///< Stores the drawing options.
  pqChartAxisModel *Model;          ///< Stores the list of labels.
  pqChartPixelScale *Scale;         ///< Stores the pixel-value scale.
  const pqChartAxis *AtMin;         ///< Stores the axis at the min.
  const pqChartAxis *AtMax;         ///< Stores the axis at the max.
  const pqChartAxis *Across;        ///< Stores the parallel axis.
  const pqChartContentsSpace *Zoom; ///< Stores the contents space.
  AxisLocation Location;            ///< Stores the axis location.
};

#endif
