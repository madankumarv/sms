<?php
/**
 * Returns a persistent variable.
 *
 * Case-sensitivity of the variable_* functions depends on the database
 * collation used. To avoid problems, always use lower case for persistent
 * variable names.
 *
 * @param $name
 *   The name of the variable to return.
 * @param $default
 *   The default value to use if this variable has never been set.
 *
 * @return
 *   The value of the variable. Unserialization is taken care of as necessary.
 *
 * @see variable_del()
 * @see variable_set()
 */
function variable_get($name, $default = NULL) {
    global $conf;
    
    return isset($conf[$name]) ? $conf[$name] : $default;
}

/**
 * Sets a persistent variable.
 *
 * Case-sensitivity of the variable_* functions depends on the database
 * collation used. To avoid problems, always use lower case for persistent
 * variable names.
 *
 * @param $name
 *   The name of the variable to set.
 * @param $value
 *   The value to set. This can be any PHP data type; these functions take care
 *   of serialization as necessary.
 *
 * @see variable_del()
 * @see variable_get()
 */
function variable_set($name, $value) {
    global $conf;
    
    db_merge('variable')->key(array('name' => $name))->fields(array('value' => serialize($value)))->execute();
    
    cache_clear_all('variables', 'cache_bootstrap');
    
    $conf[$name] = $value;
}

/**
 * Unsets a persistent variable.
 *
 * Case-sensitivity of the variable_* functions depends on the database
 * collation used. To avoid problems, always use lower case for persistent
 * variable names.
 *
 * @param $name
 *   The name of the variable to undefine.
 *
 * @see variable_get()
 * @see variable_set()
 */
function variable_del($name) {
    global $conf;
    
    db_delete('variable')
    ->condition('name', $name)
    ->execute();
    cache_clear_all('variables', 'cache_bootstrap');
    
    unset($conf[$name]);
}