#ifndef PLUGIN_VICTRONENERGY_H
#define PLUGIN_VICTRONENERGY_H

#include <plugins/plugincore/plugin.h>
#include <plugins/pluginload/plugininterface.h>
#include <tagsystem/tagsocket.h>
#include <tagsystem/tag.h>

#include <QObject>
#include <memory>
#include <vector>

namespace plugin{

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

  signals:
	void chargedEnergyChanged();
	void dischargedEnergyChanged();

  private slots:
	void onChargedEnergyChanged(double value);
	void onDischargedEnergyChanged(double value);

  private:
	std::shared_ptr<Tag> chargedTodayTag_;
	std::shared_ptr<Tag> dischargedTodayTag_;

	std::shared_ptr<TagSocket> chargedEnergyTagSocket_;
	std::shared_ptr<TagSocket> dischargedEnergyTagSocket_;

	// total updated at midnight
	int totalChargedEnergy_ = -1;
	int totalDischargedEnergy_ = -1;
	// dayly
	int chargedEnergy_ = 0;
	int dischargedEnergy_ = 0;
};

class VictronEnergy : public Plugin
{
    Q_OBJECT
    //
    Q_PLUGIN_METADATA(IID "june.plugin.victronenergy")
public:
    VictronEnergy(QObject *parent = nullptr) : Plugin(){}

    bool initialize() final;

protected slots:
    void mainloop() final;

private slots:
  void updateDaylyChaged();
  void updateDaylyDischarged();

private:
  void updateDailyEnergyUse();
  void resetValues();

  std::shared_ptr<Tag> victronTotalChargedTodayTag_;
  std::shared_ptr<Tag> victronTotalDiscargedTodayTag_;
  std::shared_ptr<Tag> victronTotalEneryUseToday_;

  std::vector<std::unique_ptr<Battery>> batteries_;

  int currentDay_ = -1;
};

}

#endif // VICTRONENERGY_H
