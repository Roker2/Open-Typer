/*
 * QmlUtils.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
 *
 * Open-Typer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Open-Typer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open-Typer. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QMenuBar>
#include <QStyle>
#ifndef Q_OS_WASM
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QTextEdit>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#endif
#include "QmlUtils.h"
#include "ThemeEngine.h"
#include "ConfigParser.h"

/*! Sets QML root item. */
void QmlUtils::setBlurSource(QQuickItem *item)
{
	m_blurSource = item;
	emit blurSourceChanged(item);
}

/*! Returns QML root item. */
QQuickItem *QmlUtils::blurSource(void)
{
	return m_blurSource;
}

/*! Returns true if the application uses native menu bar. */
bool QmlUtils::nativeMenuBar(void)
{
	QMenuBar menuBar;
	return menuBar.isNativeMenuBar();
}

/*! Returns major version of Qt. */
int QmlUtils::qtVersionMajor(void)
{
	return QT_VERSION_MAJOR;
}

/*! Returns minor version of Qt. */
int QmlUtils::qtVersionMinor(void)
{
	return QT_VERSION_MINOR;
}

/*! Returns patch version of Qt. */
int QmlUtils::qtVersionPatch(void)
{
	return QT_VERSION_PATCH;
}

/*! Returns true if the platform is WebAssembly. */
bool QmlUtils::osWasm(void)
{
#ifdef Q_OS_WASM
	return true;
#else
	return false;
#endif
}

// TODO: Remove this after dropping Qt 5.9 support
/*! Emits menuBarReloadTriggered() to fix empty menu bar in Qt 5.9. */
void QmlUtils::reloadMenuBar(void)
{
	emit menuBarReloadTriggered();
}

/*! Prints the exercise text. */
void QmlUtils::printExercise(int lesson, int sublesson, int exercise, QString text)
{
#ifndef Q_OS_WASM
	// Set up printer
	QPrinter printer(QPrinter::HighResolution);
	QPrinter *printerPtr = &printer;
	QPrintPreviewDialog dialog(&printer);
	auto dialogPtr = &dialog;
	QObject::connect(&dialog, &QPrintPreviewDialog::paintRequested, &dialog, [printerPtr, dialogPtr, lesson, sublesson, exercise, text]() {
		// Print
		printerPtr->setPageMargins(QMarginsF(25, 25, 15, 25), QPageLayout::Millimeter);
		QPainter painter;
		painter.begin(printerPtr);
		QFont font = ThemeEngine::font();
		painter.setFont(font);
		QTextEdit textEdit; // is there any other way to get rid of line wrap?
		textEdit.setPlainText(text);
		textEdit.setLineWrapMode(QTextEdit::NoWrap);
		QTextDocument *document = textEdit.document()->clone(dialogPtr);
		QString title = "";
		if(lesson + sublesson + exercise != 0)
			title = QString("<u>%1 / %2 / %3</u><br><br>").arg(ConfigParser::lessonTr(lesson), ConfigParser::sublessonName(sublesson), ConfigParser::exerciseTr(exercise));
		document->setHtml(QString("<body>%1%2</body>").arg(title, text.toHtmlEscaped().replace("\n", "<br>")));
		font.setPointSize(50);
		document->adjustSize();
		document->setDefaultFont(font);
		document->documentLayout()->setPaintDevice(printerPtr);
		document->setDefaultStyleSheet("body { color: black; }");
		document->setPageSize(printerPtr->pageRect(QPrinter::DevicePixel).size());
		double scale = printerPtr->pageRect(QPrinter::DevicePixel).width() / double(document->size().width());
		int fontHeight = QFontMetrics(painter.font(), printerPtr).height();
		QStringList lines = document->toHtml().split("<br>");
		int relativeLine = 0, page = 0, fromPage = printerPtr->fromPage() - 1, toPage = printerPtr->toPage() - 1;
		for(int i = 0; i < lines.count(); i++)
		{
			int rangeEnd = toPage;
			if(rangeEnd == -1)
				rangeEnd = page + 1;
			if(fontHeight * relativeLine > printerPtr->pageRect(QPrinter::DevicePixel).height())
			{
				if(((page + 1 >= fromPage) && (page + 1 <= rangeEnd)) && ((page >= fromPage) && (page <= rangeEnd)))
					printerPtr->newPage();
				relativeLine = 0;
				page++;
			}
			document->setHtml(lines[i]);
			if((page >= fromPage) && (page <= rangeEnd))
			{
				painter.resetTransform();
				painter.scale(scale, scale);
				painter.translate(0, fontHeight * relativeLine);
				document->drawContents(&painter);
			}
			relativeLine++;
		}
		painter.end();
	});
	dialog.exec();
#endif // Q_OS_WASM
}

/*! Prints the text of a custom exercise. */
void QmlUtils::printExercise(QString text)
{
	printExercise(0, 0, 0, text);
}

/*! Returns the given message box standard icon. */
QPixmap QmlUtils::standardIcon(QmlUtils::StandardIcon icon)
{
	QStyle *style = QApplication::style();
	int iconSize = style->pixelMetric(QStyle::PM_MessageBoxIconSize, 0, nullptr);
	QIcon tmpIcon;
	switch(icon)
	{
		case StandardIcon::Information:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxInformation, 0, nullptr);
			break;
		case StandardIcon::Warning:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxWarning, 0, nullptr);
			break;
		case StandardIcon::Critical:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxCritical, 0, nullptr);
			break;
		case StandardIcon::Question:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxQuestion, 0, nullptr);
			break;
		default:
			break;
	}
	return tmpIcon.pixmap(QSize(iconSize, iconSize));
}

/*! Returns the given message box standard icon as a string loadable from QML. \see standardIcon(). */
QString QmlUtils::standardIconStr(StandardIcon icon)
{
	return convertPixmap(standardIcon(icon));
}

/*! Saves the given pixmap in a string. */
QString QmlUtils::convertPixmap(QPixmap pixmap)
{
	QByteArray pixmapData = "PNG";
	QBuffer buffer(&pixmapData);
	buffer.open(QBuffer::WriteOnly);
	pixmap.save(&buffer, "PNG");
	QString imageStr = "data:image/png;base64,";
	imageStr += QString::fromLatin1(pixmapData.toBase64().data());
	return imageStr;
}