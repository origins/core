DROP TABLE IF EXISTS `character_action`;
CREATE TABLE `character_action` (
`guid` INT(11) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
`spec` TINYINT(3) NOT NULL DEFAULT '0',
`button` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
`action` INT(11) UNSIGNED NOT NULL DEFAULT '0',
`type` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
PRIMARY KEY  (`guid`,`spec`,`button`)
) ENGINE=INNODB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Player System';