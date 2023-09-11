DELETE FROM `command` WHERE `name` IN ('xp', 'xp set', 'xp view', 'xp default', 'xp enable', 'xp disable');
INSERT INTO `command` (`name`, `permission`, `help`) VALUES 
('xp', 195, 'Syntax: .xp $subcommand\nType .help xp to see a list of subcommands\nor .help xp $subcommand to see info on the subcommand.'),
('xp set', 195, 'Syntax: .xp set $rate\nSet your custom XP rate.'),
('xp view', 195, 'Syntax: .xp view\nView your current XP rate.'),
('xp default', 195, 'Syntax: .xp default\nSet your custom XP rate to the default value'),
('xp enable', 195, 'Syntax: .xp enable\nEnable the custom XP rate.'),
('xp disable', 195, 'Syntax: .xp disable\nDisable the custom XP rate.');