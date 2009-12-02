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
 * Implements hook_form_submit().
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