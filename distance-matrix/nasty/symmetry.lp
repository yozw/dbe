# Additional constraints to impose symmetry.

subject to symmbreak1: d['V', 'rx'] <= d['V', 'sx'] - 1;
subject to symmbreak2: d['V', 'sx'] <= d['V', 'tx'] - 1;
subject to symmbreak3: d['H', 'rx'] <= d['H', 'ry'] - 1;
subject to symmbreak4: d['H', 'ry'] <= d['H', 'rz'] - 1;

subject to se1: d['H', 'rx'] = d['H', 'sx'];
subject to se2: d['H', 'rx'] = d['H', 'tx'];
subject to se3: d['H', 'ry'] = d['H', 'sy'];
subject to se4: d['H', 'ry'] = d['H', 'ty'];
subject to se5: d['H', 'rz'] = d['H', 'sz'];
subject to se6: d['H', 'rz'] = d['H', 'tz'];

subject to sv1: d['V', 'rx'] = d['V', 'ry'];
subject to sv2: d['V', 'rx'] = d['V', 'rz'];
subject to sv3: d['V', 'sx'] = d['V', 'sy'];
subject to sv4: d['V', 'sx'] = d['V', 'sz'];
subject to sv5: d['V', 'tx'] = d['V', 'ty'];
subject to sv6: d['V', 'tx'] = d['V', 'tz'];

subject to sr1: d['rx', 'ry'] = d['sx', 'sy'];
subject to sr2: d['tx', 'ty'] = d['sx', 'sy'];
subject to sr3: d['rx', 'rz'] = d['sx', 'sz'];
subject to sr4: d['tx', 'tz'] = d['sx', 'sz'];
subject to sr5: d['ry', 'rz'] = d['sy', 'sz'];
subject to sr6: d['ty', 'tz'] = d['sy', 'sz'];

subject to sc1: d['rx', 'sx'] = d['ry', 'sy'];
subject to sc2: d['rx', 'sx'] = d['rz', 'sz'];
subject to sc3: d['rx', 'tx'] = d['ry', 'ty'];
subject to sc4: d['rx', 'tx'] = d['rz', 'tz'];
subject to sc5: d['sx', 'tx'] = d['sy', 'ty'];
subject to sc6: d['sx', 'tx'] = d['sz', 'tz'];
