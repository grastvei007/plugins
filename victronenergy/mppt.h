#ifndef MPPT_H
#define MPPT_H

#include <QObject>

class TagList;
class TagSocket;

#include <memory>

namespace plugin {

class Mppt : public QObject
{
	Q_OBJECT
  public:
	Mppt(TagList *taglist, const QString &serialNumber);

	void reset();
	double yield() const { return daylyYield_; }

  private slots:
	void onYieldChanged(double value);

  private:
	std::shared_ptr<TagSocket> yieldTagSocket_;

	double daylyYield_ = 0;
};

} // namespace plugin
#endif // MPPT_H
