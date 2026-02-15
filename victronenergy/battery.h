#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>
#include <tagsystem/tag.h>
#include <tagsystem/taglist.h>
#include <tagsystem/tagsocket.h>

namespace plugin {

class Battery : public QObject
{
	Q_OBJECT
  public:
	Battery(TagList *taglist,
			const QString &subsystem,
			const QString &name,
			const QString &batteyName,
			QObject *parent = nullptr);

	void resetValues();

	int chargedEnergy() const { return chargedEnergy_; }
	int dischargedEnergy() const { return dischargedEnergy_; }
	double amphere() const { return amphere_; }
	int power() const { return power_; }

  signals:
	void chargedEnergyChanged();
	void dischargedEnergyChanged();
	void amphereChanged();
	void powerChanged();

  private slots:
	void onChargedEnergyChanged(double value);
	void onDischargedEnergyChanged(double value);
	void onAmphereChanged(double value);
	void onPowerChanged(int value);

  private:
	std::shared_ptr<Tag> chargedTodayTag_;
	std::shared_ptr<Tag> dischargedTodayTag_;

	std::shared_ptr<TagSocket> chargedEnergyTagSocket_;
	std::shared_ptr<TagSocket> dischargedEnergyTagSocket_;
	std::shared_ptr<TagSocket> amphereTagSocket_;
	std::shared_ptr<TagSocket> powerTagSocket_;

	// total updated at midnight
	int totalChargedEnergy_ = -1;
	int totalDischargedEnergy_ = -1;
	// dayly
	int chargedEnergy_ = 0;
	int dischargedEnergy_ = 0;
	double amphere_ = 0.0;
	int power_ = 0;
};

} // namespace plugin

#endif // BATTERY_H
