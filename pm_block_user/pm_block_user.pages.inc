<?php
// $Id$

/**
 * @file
 * User menu callbacks for pm_block_user.module.
 */

function pm_block_user_form($form_state, $author) {
  global $user;

  $form['author'] = array(
    '#type' => 'value',
    '#value' => $author->uid,
  );
    $form['recipient'] = array(
    '#type' => 'value',
    '#value' => $user->uid,
  );
  $form['author_name'] = array(
    '#type' => 'value',
    '#value' => $author->name,
  );
  $form['destination'] = array(
    '#type' => 'value',
    '#value' => isset($_GET['destination']) ? $_GET['destination'] : 'messages/',
  );

  if (db_result(db_query('SELECT COUNT(recipient) FROM {pm_block_user} WHERE author = %d AND recipient = %d', $author->uid, $user->uid))) {
    $form['block_action'] = array(
      '#type' => 'value',
      '#value' => 'unblock_user',
    );
    return confirm_form($form,
      t('You have previously blocked "@author" from sending you any more messages. Are you sure you want to unblock this user?', array('@author' => $author->name)),
      isset($_GET['destination']) ? $_GET['destination'] : 'messages/',
      t('This action cannot be undone.'),
      t('Unblock @author', array('@author' => $author->name)),
      t('Cancel')
    );
  }
  else {
    $form['block_action'] = array(
      '#type' => 'value',
      '#value' => 'block_user',
    );
    return confirm_form($form,
      t('Are you sure you want to block "@author" from sending you any more messages?', array('@author' => $author->name)),
      isset($_GET['destination']) ? $_GET['destination'] : 'messages/',
      '',
      t('Block @author', array('@author' => $author->name)),
      t('Cancel')
    );
  }
}

/**
 * Submit callback for block user confirmation form.
 */
function pm_block_user_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    switch ($form_state['values']['block_action']) {
      case 'block_user':
        db_query('INSERT INTO {pm_block_user} (author, recipient) VALUES (%d, %d)', $form_state['values']['author'], $form_state['values']['recipient']);
        drupal_set_message(t('@author has been blocked from sending you any further messages.', array('@author' => $form_state['values']['author_name'])));
      break;
      case 'unblock_user':
        db_query('DELETE FROM {pm_block_user} WHERE author = %d AND recipient = %d', $form_state['values']['author'], $form_state['values']['recipient']);
        drupal_set_message(t('@author is now allowed to send you new messages.', array('@author' => $form_state['values']['author_name'])));
      break;
    }
  }
  $form_state['redirect'] = $form_state['values']['destination'];
}

/**
 * Formbuilder function to build a simple form to block users.
 */
function pm_block_user_list() {
  global $user;

  $form['new'] = array(
    '#type'   => 'fieldset',
    '#title'  => t('Block a user'),
  );
  $form['new']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#autocomplete_path' => 'messages/user-name-autocomplete',
    '#description'        => t('Separate multiple names with commas.'),
  );
  $form['new']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Block user'),
    '#submit' => array('pm_block_user_block_submit'),
  );

  $header = array(
    array(
      'data' => t('Username'),
      'field' => 'u.name',
      'sort'  => 'asc',
      ),
    array(
      'data' => t('Operations'),
      ),
  );

  $result = pager_query('SELECT * FROM {pm_block_user} pmb INNER JOIN {users} u ON (u.uid = pmb.author) WHERE pmb.recipient = %d ' . tablesort_sql($header), 20, 0, NULL, $user->uid);
  $rows = array();
  while ($row = db_fetch_object($result)) {
    $rows[] = array(
      theme('username', user_load($row->author)),
      l(t('unblock'), 'messages/block/' . $row->author, array('query' => drupal_get_destination())),
    );
  }
  $form['#header'] = $header;
  $form['#rows'] = $rows;
  return $form;
}

/**
 * Submit callback for block user form.
 */
function pm_block_user_block_submit($form, &$form_state) {
  global $user;
  list($accounts, $invalid) = _privatemsg_parse_userstring($form_state['values']['name']);
  if (!empty($accounts)) {
    foreach ($accounts as $account) {
      db_query('INSERT INTO {pm_block_user} (author, recipient) VALUES (%d, %d)', $account->uid, $user->uid);
      drupal_set_message(t('!author has been blocked from sending you any further messages.', array('!author' => theme('username', $account))));
    }
  }
}

/**
 * Theme function to theme the blocked user listing.
 */
function theme_pm_block_user_list($form) {
  return drupal_render($form) . theme('table', $form['#header'], $form['#rows']) . theme('pager');
}